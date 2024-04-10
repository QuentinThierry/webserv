#include "Cluster.class.hpp"

Cluster::Cluster(std::vector<Server> servers)
{
	_max_fd = 0;
	for (unsigned int i = 0; i < servers.size(); i++)
		addServer(servers[i]);
}

Socket const * Cluster::find_same_config_server(Server const &server)
{
	for (t_const_iter_servers it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->getServer().getHost() == server.getHost()
				&& it->getServer().getPort() == server.getPort())
			return &(*it);
	}
	return NULL;
}


void Cluster::addServer(Server const &server)
{
	Socket const * same_config = find_same_config_server(server);
	if (same_config == NULL)
	{
		Socket new_socket = Socket(server);
		if (new_socket.getFd() == -1)
			return ;
		_servers.push_back(new_socket);
		if (new_socket.getFd() > _max_fd)
			_max_fd = new_socket.getFd();
		if (_max_fd >= FD_SETSIZE)
		{
			std::cout << "Error : Too many socket" << std::endl;
			return ;
		}
	}
	else
	{
		Socket copy_socket = Socket(server, *same_config);
		_servers.push_back(copy_socket);
	}
}

void Cluster::init_set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds)
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	FD_ZERO(exceptfds);
	for (t_const_iter_servers it = _servers.begin(); it != _servers.end(); it++)
	{
		FD_SET(it->getFd(), exceptfds);
		FD_SET(it->getFd(), readfds);
	}
	for (t_const_iter_map_read it = _map_read.begin(); it != _map_read.end(); it++)
	{
		FD_SET(it->first, exceptfds);
		FD_SET(it->first, readfds);
	}
}

void Cluster::print_set(fd_set *fds, std::string str)
{
	std::cout << str << " SET :" << std::endl;
	for (t_const_iter_servers it = _servers.begin(); it != _servers.end(); it++)
	{
		if (FD_ISSET(it->getFd(), fds))
			std::cout << it->getFd() << std::endl;
	}
	for (t_const_iter_map_read it = _map_read.begin(); it != _map_read.end(); it++)
	{
		if (FD_ISSET(it->first, fds))
			std::cout << it->first << " read" << std::endl;
	}
	std::cout << "END" << std::endl;
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
		init_set_fds(&readfds, &writefds, &exceptfds);
		print_set(&readfds, "READ");
		print_set(&writefds, "WRITE");
		print_set(&exceptfds, "EXCEPT");
		std::cout << " ----- SELECT() ---- " << std::endl;
		int nb_fds = select(_max_fd + 1, &readfds, &writefds, &exceptfds, &timeout);
		print_set(&readfds, "READ");
		print_set(&writefds, "WRITE");
		print_set(&exceptfds, "EXCEPT");
		if (nb_fds == -1)
			return ; //??
		if (nb_fds == 0)
			continue;
		for (t_iter_servers it = _servers.begin(); it != _servers.end(); it++)
		{
			if (FD_ISSET(it->getFd(), &exceptfds))
				std::cout << "ERROR" << std::endl;
				// error(it);
			else if (FD_ISSET(it->getFd(), &readfds))
				acceptNewConnection(*it);
		}
		for (t_iter_map_read it = _map_read.begin(); it != _map_read.end(); it++)
		{
			if (FD_ISSET(it->first, &exceptfds))
				std::cout << "ERROR" << std::endl;
				// error(it);
			// else if (FD_ISSET(it->first, &readfds))
			// 	it->second.read_header(it->first);
			// else if (FD_ISSET(it->first, writefds))
			// 	writerequest(it);
		}
		std::cout << " ----- fin ---- " << std::endl;
	}
}

void Cluster::acceptNewConnection(Socket const & socket)
{
	int new_fd = accept(socket.getFd(), (sockaddr*)&socket.getAddresse(),
			(socklen_t*)&socket.getSizeAddresse());
	if (new_fd == -1)
	{
		std::cout << "Error: accept failed" << std::endl;
		std::perror("Error");
		return;
	}
	_map_read.insert(std::make_pair(new_fd, HttpExchange(socket)));
	if (new_fd > _max_fd)
		_max_fd = new_fd;
	if (_max_fd >= FD_SETSIZE)
	{
		std::cout << "Error : Too many socket" << std::endl;
		return ;
	}
}


