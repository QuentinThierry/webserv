#include "Cluster.class.hpp"
#include "ConfException.class.hpp"
#include "utils.hpp"

Cluster::Cluster():_max_fd(0), _close_connection(false){};

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
		_close_connection = copy._close_connection;
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
	_close_connection = false;
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
			protected_write(g_err_log_fd, error_message_server(new_socket.getServer(),
					"Error: Too many servers, ignore"));
			return ;
		}
		if (new_socket.getFd() > _max_fd)
			_max_fd = new_socket.getFd();
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
		protected_write(g_err_log_fd, std::string("Error: ") + std::strerror(errno));
		throw ExceptionHttpStatusCode(HTTP_500);
	}
	for (unsigned int i = 0; i < _map_sockets.size(); i++)
	{
		if (_map_sockets.at(i).second.getAcceptRequestTime().tv_sec + TIMEOUT_SEC < time.tv_sec)
		{
			protected_write(g_err_log_fd, error_message_server(_map_sockets.at(i).second.getSocket().getServer(),
					std::string("Error: Request timeout")));
			closeConnection(_map_sockets.at(i).first);
		}
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
		////std::cout << " ----- SELECT() ---- " << std::endl;
		int nb_fds = select(_max_fd + 1, &readfds, &writefds, NULL, &timeout);
		if (nb_fds == -1)
			ThrowMisc(strerror(errno));
		if (nb_fds == 0)
			continue ;
		for (t_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
		{
			if (FD_ISSET(it->getFd(), &readfds))
			{
				////std::cout << "new connection" << std::endl;
				_acceptNewConnection(*it);
				break;
			}
		}
		for (t_const_iter_map_cgi it = _map_cgi.begin(); it != _map_cgi.end(); it++)
		{
			if (FD_ISSET(it->first->getReadPipe(), &readfds))
			{
				////std::cout << "read cgi "<<it->first->getReadPipe() << std::endl;
				it->second->readCgi(findFd(_map_sockets, it->second), *this);
				break;
			}
			else if (*it->second->getRequest().getMethod() == "POST"
					&& FD_ISSET(it->first->getWritePipe(), &writefds))
			{
				////std::cout << "write cgi "<<it->first->getWritePipe() << std::endl;
				it->second->writeCgi(findFd(_map_sockets, it->second), *this);
				break;
			}
		}
		for (unsigned int i = 0; i < _map_sockets.size() + (_close_connection == true); i++)
		{
			if (_close_connection == true)
			{
				if (i > 0)
					i--;
				_close_connection = false;
			}
			if (FD_ISSET(_map_sockets.at(i).first, &writefds))
			{
				////std::cout << "write data" << std::endl;
				_map_sockets.at(i).second.writeSocket(_map_sockets.at(i).first, *this);
				break ;
			}
			else if (FD_ISSET(_map_sockets.at(i).first, &readfds))
			{
				////std::cout << "read data" << std::endl;
				_map_sockets.at(i).second.readSocket(_map_sockets.at(i).first, *this);
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
		protected_write(g_err_log_fd, error_message_server(socket.getServer(),
					std::string("Error: accept() new connection ") + std::strerror(errno)));
		return;
	}
	if (new_fd >= FD_SETSIZE)
	{
		close(new_fd);
		protected_write(g_err_log_fd, error_message_server(socket.getServer(),
					"Error: Too many servers, ignore new connection to"));
		return ;
	}
	if (new_fd > _max_fd)
		_max_fd = new_fd;
	_map_sockets.push_back(std::make_pair(new_fd, HttpExchange(socket)));
}

Socket const *Cluster::getMatchingSocket(int fd, std::string server_name) const
{
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
	////std::cout << "close connection\n";
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
			if (it == _map_sockets.begin())
				_map_sockets.pop_front();
			else
				_map_sockets.erase(it);
			_close_connection = true;
			break;
		}
	}
}

void Cluster::addCgi(Cgi *cgi, HttpExchange *httpExchange)
{
	////std::cout << "add cgi" << std::endl;
	if (cgi->getReadPipe() > _max_fd)
		_max_fd = cgi->getReadPipe();
	if (cgi->getWritePipe() > _max_fd)
		_max_fd = cgi->getWritePipe();
	_map_cgi.push_back(std::make_pair(cgi, httpExchange));
}
