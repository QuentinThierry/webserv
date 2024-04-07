#include "SocketManager.class.hpp"
#define NB_QUEUE_PORT 5
#define MAX_NB_SOCKET 1024

int create_socket(uint16_t port, uint32_t host)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("Error socket()");
		return -1;
	}
	struct sockaddr_in addr;
	socklen_t sizeaddr = sizeof(addr);
	memset(&addr, 0, sizeaddr);
	// getsockname(sockfd, (sockaddr*)&addr, (socklen_t*)&sizeaddr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(host); //uint32_t
	addr.sin_port = htons(port); //uint16_t
	if (bind(sockfd, (const sockaddr*)&addr, sizeaddr) == -1)
	{
		perror("Error bind()");
		close(sockfd);
		return -1;
	}
	if (listen(sockfd, NB_QUEUE_PORT) == -1)
	{
		perror("Error listen()");
		close(sockfd);
		return -1;
	}
	return sockfd;
}

void SocketManager::addServer(Server const &server)
{
	if (_server_fd.size() + 1 > MAX_NB_SOCKET)
	{
		std::cout << "Error : Too many socket" << std::endl;
		return;
	}
	int socketfd = create_socket(server.fd, server.host);
	if (socketfd == -1)
		return;
	_server_fd.push_back(socketfd);
	_map_server.push_back(std::make_pair(socketfd, server));
	if (socketfd > _max_fd)
		_max_fd = socketfd;
}

void SocketManager::init_set(fd_set *readfds, fd_set *writefds, fd_set *exceptfds)
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	FD_ZERO(exceptfds);
	for (iter_map_server it = _map_server.begin(); it != _map_server.end(); it++)
	{
		FD_SET(it->first, exceptfds);
		FD_SET(it->first, readfds);
	}
	for (iter_map_client it = _map_client.begin(); it != _map_client.end(); it++)
	{
		FD_SET(it->first, exceptfds);
		if (it->second.status == READ)
			FD_SET(it->first, readfds);
		if (it->second.status == WRITE)
			FD_SET(it->first, writefds);
	}
}

static void _print_set(fd_set *fds)
{
	std::cout << "SET :";
	for (iter_map_server it = _map_server.begin(); it != _map_server.end(); it++)
		if (FD_ISSET(it->first, fds))
			std::cout << it->first << std::endl;
	for (iter_map_client it = _map_client.begin(); it != _map_client.end(); it++)
		if (FD_ISSET(it->first, fds))
			std::cout << it->first << std::endl;
	std::cout << "END" << std::endl;
}

void SocketManager::runServer()
{
	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	struct timeval timeout;
	while (true)
	{
		timeout.tv_sec = 3; // 0
		timeout.tv_usec = 0;
		init_set(readfds, writefds, exceptfds);
		int nb_fds = select(_max_fd + 1, readfds, writefds, exceptfds, timeout);
		_print_set(readfds);
		_print_set(writefds);
		_print_set(exceptfds);
		if (nb_fds == -1)
			return ; //??
		if (nb_fds == 0)
			continue;
		for (iter_map_server it = _map_server.begin(); it != _map_server.end(); it++)
		{
			if (FD_ISSET(it->first, exceptfds))
				error(it);
			else if (FD_ISSET(it->first, readfds))
				acceptNewConnection(*it);
		}
		for (iter_map_client it = _map_client.begin(); it != _map_client.end(); it++)
		{
			if (FD_ISSET(it->first, exceptfds))
				error(it);
			else if (FD_ISSET(it->first, readfds))
				readrequest(it);
			else if (FD_ISSET(it->first, writefds))
				writerequest(it);
		}
	}
}

void SocketManager::acceptNewConnection(std::pair<int, Server const &>  const & server)
{
	Client new_client;

	int new_fd = accept(server.first, (sockaddr*)&addr, (socklen_t*)&sizeaddr);
}
