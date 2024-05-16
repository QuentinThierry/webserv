#include "Cluster.class.hpp"
#include "util.hpp"

Cluster::Cluster():_max_fd(0), _close_connection(false){};

Cluster::Cluster(Cluster const &copy)
{
	*this = copy;
}

Cluster & Cluster::operator=(Cluster const &copy)
{
	_sockets = copy._sockets;
	_map_sockets = copy._map_sockets;
	_fd_write = copy._fd_write;
	_max_fd = copy._max_fd;
	_close_connection = copy._close_connection;
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

Socket const * Cluster::_find_same_config_server(Server const &server) const
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
	Socket const * same_config = _find_same_config_server(server);
	if (same_config == NULL)
	{
		Socket new_socket(server);
		if (new_socket.getFd() == -1)
			return ;
		if (new_socket.getFd() > _max_fd)
			_max_fd = new_socket.getFd();
		if (_max_fd >= FD_SETSIZE)
		{
			close(new_socket.getFd());
			protected_write(g_err_log_fd, error_message_server(new_socket.getServer(),
					"Error: Too many servers, igonre"));
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

void Cluster::_init_set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds) const
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	FD_ZERO(exceptfds);
	for (t_const_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
	{
		FD_SET(it->getFd(), exceptfds);
		FD_SET(it->getFd(), readfds);
	}
	unsigned int index = 0;
	for (t_const_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		FD_SET(it->first, exceptfds);
		if (_fd_write.size() > index && it->first == _fd_write[index])
		{
			FD_SET(it->first, writefds);
			index++;
		}
		else
			FD_SET(it->first, readfds);
	}
}

void Cluster::_print_set(fd_set *fds, std::string str) const
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

void Cluster::_check_timeout()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	for (unsigned int i = 0; i < _map_sockets.size(); i++)
	{
		if (_map_sockets.at(i).second.getAcceptRequestTime().tv_sec + TIMEOUT_SEC < time.tv_sec)
		{
			std::cout << "timeout " << _map_sockets.at(i).first << std::endl;//!close connection or send error to client
			closeConnection(_map_sockets.at(i).first);
		}
	}
}

void Cluster::runServer()
{
	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	struct timeval timeout;
	while (true)
	{
		timeout.tv_sec = 3; // 0
		timeout.tv_usec = 0;
		_init_set_fds(&readfds, &writefds, &exceptfds);
		// _print_set(&readfds, "READ");
		// _print_set(&writefds, "WRITE");
		// _print_set(&exceptfds, "EXCEPT");
		std::cout << " ----- SELECT() ---- " << std::endl;
		int nb_fds = select(_max_fd + 1, &readfds, &writefds, &exceptfds, &timeout);
		// _print_set(&readfds, "READ");
		// _print_set(&writefds, "WRITE");
		// _print_set(&exceptfds, "EXCEPT");
		if (nb_fds == -1)
			return ; //??
		if (nb_fds == 0)
			continue;
		for (t_iter_sockets it = _sockets.begin(); it != _sockets.end(); it++)
		{
			if (FD_ISSET(it->getFd(), &exceptfds))
				std::cout << "ERROR" << std::endl;
			else if (FD_ISSET(it->getFd(), &readfds))
			{
				std::cout << "new connection" << std::endl;
				_acceptNewConnection(*it);
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
			if (FD_ISSET(_map_sockets.at(i).first, &exceptfds))
			{
				std::cout << "ERROR" << std::endl;
				closeConnection(_map_sockets.at(i).first);
			}
			else if (FD_ISSET(_map_sockets.at(i).first, &writefds))
			{
				std::cout << "write data" << std::endl;
				_map_sockets.at(i).second.writeSocket(_map_sockets.at(i).first, *this);
				break ;
			}
			else if (FD_ISSET(_map_sockets.at(i).first, &readfds))
			{
				std::cout << "read data" << std::endl;
				_map_sockets.at(i).second.readSocket(_map_sockets.at(i).first, *this);
				break ;
			}
		}
		_check_timeout();
		// std::cout << " ----- fin ---- " << std::endl;
	}
}

void Cluster::_acceptNewConnection(Socket const & socket)
{
	int new_fd = accept(socket.getFd(), (sockaddr*)&socket.getAddresse(),
			(socklen_t*)&socket.getSizeAddresse());
	if (new_fd == -1)
	{
		protected_write(g_err_log_fd, error_message_server(socket.getServer(),
					std::string("Error: accept() new connection ") + std::strerror(errno)));
		return;
	}
	if (new_fd > _max_fd)
		_max_fd = new_fd;
	if (_max_fd >= FD_SETSIZE)
	{
		close(new_fd);
		protected_write(g_err_log_fd, error_message_server(socket.getServer(),
					"Error: Too many servers, igonre new connection to"));
		return ;
	}
	_map_sockets.push_back(std::make_pair(new_fd, HttpExchange(socket)));
}

Socket const *Cluster::get_matching_socket(int fd, std::string server_name) const
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
	_fd_write.push_back(fd);
}

void Cluster::closeConnection(int fd)
{
	std::cout << "close connection\n";
	std::vector<int>::iterator pos = std::find(_fd_write.begin(), _fd_write.end(), fd);
	if (pos != _fd_write.end())
	{
		_fd_write.erase(pos);
	}
	for (t_iter_map_sockets it = _map_sockets.begin(); it != _map_sockets.end(); it++)
	{
		if (it->first == fd)
		{
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


