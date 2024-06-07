#include "Cluster.class.hpp"
#include "ConfException.class.hpp"
#include "utils.hpp"

Cluster::Cluster():_max_fd(0) {};

Cluster::Cluster(Cluster const &copy)
{
	*this = copy;
}

Cluster & Cluster::operator=(Cluster const &copy)
{
	if (this != &copy)
	{
		_sockets = copy._sockets;
		_map_sockets = copy._map_sockets;
		_fd_write = copy._fd_write;
		_max_fd = copy._max_fd;
	}
	return *this;
}

Cluster::~Cluster()
{
	for (t_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		close(it->first);
	}
	for (t_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
		close(it->getFd());
}

Cluster::Cluster(std::vector<Server> servers)
{
	_max_fd = 0;
	for (unsigned int i = 0; i < servers.size(); i++)
		_addServer(servers[i]);
}

Socket const * Cluster::_findSameConfigServer(Server const &server) const
{
	for (t_const_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
	{
		if (it->getServer().getHost() == server.getHost()
				&& it->getServer().getPort() == server.getPort())
			return &(*it);
	}
	return NULL;
}

void Cluster::_addServer(Server const &server)
{
	Socket const * same_config = _findSameConfigServer(server);
	if (same_config == NULL)
	{
		Socket new_socket(server);
		if (new_socket.getFd() == -1)
			return ;
		if (new_socket.getFd() >= FD_SETSIZE)
		{
			close(new_socket.getFd());
			protected_write_log(error_message_server(new_socket.getServer(),
					"Error: Too many servers, ignore"));
			return ;
		}
		_sockets.push_back(new_socket);
	}
	else
	{
		Socket copy_socket(server, *same_config);
		_sockets.push_back(copy_socket);
	}
}

void Cluster::_initSetFds(fd_set *readfds, fd_set *writefds) const
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	for (t_const_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
		FD_SET(it->getFd(), readfds);
	unsigned int index = 0;
	for (t_const_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		if (_fd_write.size() > index && it->first == _fd_write[index])
		{
			FD_SET(it->first, writefds);
			index++;
		}
		else
			FD_SET(it->first, readfds);
	}
	for (t_const_iter_map_cgi it = _map_cgi.begin(); it != _map_cgi.end(); it++)
	{
		FD_SET(it->first->getReadPipe(), readfds);
		if (*it->second->getRequest().getMethod() == "POST")
			FD_SET(it->first->getWritePipe(), writefds);
	}
}

void Cluster::_printSet(fd_set *fds, std::string str) const
{
	std::cout << str << " SET :" << std::endl;
	for (t_const_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
	{
		if (FD_ISSET(it->getFd(), fds))
			std::cout << it->getFd() << std::endl;
	}
	for (t_const_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		if (FD_ISSET(it->first, fds))
			std::cout << it->first << " accept" << std::endl;
	}
	std::cout << "END" << std::endl;
}

void Cluster::_checkTimeout()
{
	struct timeval time;

	if (gettimeofday(&time, NULL) == -1)
	{
		protected_write_log(std::string("Error: ") + std::strerror(errno));
		throw ExceptionHttpStatusCode(HTTP_500);
	}
	t_const_iter_map_sockets it = _map_sockets.begin();
	while(it != _map_sockets.end())
	{
		t_const_iter_map_sockets next_connect = ++it;
		it--;
		if (it->second.getAcceptRequestTime().tv_sec + TIMEOUT_SEC < time.tv_sec)
		{
			protected_write_log(error_message_server(it->second.getSocket().getServer(),
					std::string("Error: Request timeout")));
			closeConnection(it->first);
		}
		it = next_connect;
	}
}

static int findFd(t_map_sockets const & map_socket, HttpExchange *exchange)
{
	for (t_const_iter_map_sockets it = map_socket.begin(); it != map_socket.end(); it++)
	{
		if (&it->second == exchange)
			return it->first;
	}
	return -1;
}

void Cluster::_setMaxFd()
{
	_max_fd = 0;
	for (t_const_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
		if (it->getFd() > _max_fd)
			_max_fd = it->getFd();
	for (t_const_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		if (it->first > _max_fd)
			_max_fd = it->first;
	}
	for (t_const_iter_map_cgi it = _map_cgi.begin(); it != _map_cgi.end(); it++)
	{
		if (it->first->getReadPipe() > _max_fd)
			_max_fd = it->first->getReadPipe();
		if (*it->second->getRequest().getMethod() == "POST")
			if (it->first->getWritePipe() > _max_fd)
				_max_fd = it->first->getWritePipe();
	}
}

// static void _display_fd_valid(t_map_cgi & cgi, t_map_sockets &map_sockets, t_sockets &socket)
// {
// 	std::cout << "----------------------------------------------------------------"<<std::endl;
// 	std::cout << "fd accept: ";
// 	for (t_const_iter_sockets it = socket.begin(); it != socket.end(); it++)
// 	{
// 		std::cout << it->getFd() << " ";
// 		if (fcntl(it->getFd(), F_GETFD) == -1)
// 			std::cout << "bad fd accept :" << it->getFd() << std::endl;
// 	}
// 	std::cout << std::endl << "fd read or write: ";
// 	for (t_const_iter_map_sockets it = map_sockets.begin(); it != map_sockets.end(); it++)
// 	{
// 		std::cout << it->first << " ";
// 		if (fcntl(it->first, F_GETFD) == -1)
// 			std::cout << "bad fd read or write :" << it->first << std::endl;
// 	}
// 	std::cout << std::endl << "fd pipe: ";
// 	for (t_const_iter_map_cgi it = cgi.begin(); it != cgi.end(); it++)
// 	{
// 		std::cout <<it->first->getReadPipe() << "(R) ";
// 		if (fcntl(it->first->getReadPipe(), F_GETFD) == -1)
// 			std::cout << "bad fd read pipe:" << it->first->getReadPipe() << std::endl;
// 		if (*it->second->getRequest().getMethod() == "POST")
// 		{
// 			std::cout <<it->first->getReadPipe() << "(W) ";
// 			if (fcntl(it->first->getWritePipe(), F_GETFD) == -1)
// 				std::cout << "bad fd write pipe :" << it->first->getWritePipe() << std::endl;
// 		}
// 	}
// 	std::cout << std::endl;
// }

void Cluster::runServer()
{
	fd_set readfds;
	fd_set writefds;
	struct timeval timeout;
	while (g_exit == false)
	{
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		_initSetFds(&readfds, &writefds);
		_setMaxFd();
		int nb_fds = select(_max_fd + 1, &readfds, &writefds, NULL, &timeout);
		if (nb_fds == -1)
			ThrowMisc(strerror(errno));
		if (nb_fds == 0)
			continue ;
		for (t_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
		{
			if (FD_ISSET(it->getFd(), &readfds))
			{
				_acceptNewConnection(*it);
				break;
			}
		}
		for (t_const_iter_map_cgi it = _map_cgi.begin(); it != _map_cgi.end(); it++)
		{
			if (FD_ISSET(it->first->getReadPipe(), &readfds))
			{
				it->second->readCgi(findFd(_map_sockets, it->second), *this);
				break;
			}
			else if (*it->second->getRequest().getMethod() == "POST"
					&& FD_ISSET(it->first->getWritePipe(), &writefds))
			{
				it->second->writeCgi(findFd(_map_sockets, it->second), *this);
				break;
			}
		}
		for (t_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
		{
			if (FD_ISSET(it->first, &writefds))
			{
				it->second.writeSocket(it->first, *this);
				break ;
			}
			else if (FD_ISSET(it->first, &readfds))
			{
				it->second.readSocket(it->first, *this);
				break ;
			}
		}
		_checkTimeout();
	}
}

void Cluster::_acceptNewConnection(Socket const & socket)
{
	int new_fd = accept(socket.getFd(), (sockaddr*)&socket.getAddress(),
			(socklen_t*)&socket.getSizeAddress());
	if (new_fd == -1)
	{
		protected_write_log(error_message_server(socket.getServer(),
					std::string("Error: accept() failure for a new connection: ") + std::strerror(errno)));
		return;
	}
	if (new_fd >= FD_SETSIZE)
	{
		close(new_fd);
		protected_write_log(error_message_server(socket.getServer(),
					"Error: Too many servers, ignore new connection to"));
		return ;
	}
	_map_sockets.push_back(std::make_pair(new_fd, HttpExchange(socket)));
}

Socket const *Cluster::getMatchingSocket(int fd, std::string server_name) const
{
	size_t pos = server_name.find(':');
	if (pos != std::string::npos)
		server_name = server_name.substr(0, pos);
	for (t_const_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
	{
		std::vector<std::string> names = it->getServer().getServerName();
		if (it->getFd() == fd && std::find(names.begin(), names.end(), server_name) != names.end())
			return &(*it);
	}
	return NULL;
}

void Cluster::switchHttpExchangeToWrite(int fd)
{
	if (find(_fd_write.begin(), _fd_write.end(), fd) == _fd_write.end())
		_fd_write.push_back(fd);
}

static void remove_cgi(HttpExchange *exchange, t_map_cgi & map_cgi)
{
	for (t_iter_map_cgi it = map_cgi.begin(); it != map_cgi.end(); it++)
	{
		if (it->second == exchange)
		{
			map_cgi.erase(it);
			break;
		}
	}
}

void Cluster::closeConnection(int fd)
{
	std::vector<int>::iterator pos = std::find(_fd_write.begin(), _fd_write.end(), fd);
	if (pos != _fd_write.end())
	{
		_fd_write.erase(pos);
	}
	for (t_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		if (it->first == fd)
		{
			remove_cgi(&it->second, _map_cgi);
			close(it->first);
			_map_sockets.erase(it);
			break;
		}
	}
}

void Cluster::addCgi(Cgi *cgi, HttpExchange *httpExchange)
{
	_map_cgi.push_back(std::make_pair(cgi, httpExchange));
}
