#include "Socket.class.hpp"

Socket::Socket(Server const &server):_server(server)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
	{
		std::perror("Error socket()");
		return;
	}
	_sizeaddr = sizeof(_addr);
	memset(&_addr, 0, _sizeaddr);
	// getsockname(_fd_listening, (sockaddr*)&addr, (socklen_t*)&sizeaddr);
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(server.getHost()); //uint32_t
	_addr.sin_port = htons(server.getPort()); //uint16_t
	if (bind(_fd, (const sockaddr*)&_addr, _sizeaddr) == -1)
	{
		std::perror("Error bind()");
		close(_fd);
		_fd = -1;
		return;
	}
	if (listen(_fd, NB_QUEUE_PORT) == -1)
	{
		std::perror("Error listen()");
		close(_fd);
		_fd = -1;
		return;
	}
	std::cout << "create socket : " << server.getPort() << " " << server.getHost() << std::endl;
}

Socket::Socket(Server const &server, Socket const & socket): _fd(socket._fd),
			_addr(socket._addr), _sizeaddr(socket._sizeaddr), _server(server) {}


int const & Socket::getFd() const
{
	return _fd;
}

struct sockaddr_in const & Socket::getAddresse() const
{
	return _addr;
}

socklen_t const & Socket::getSizeAddresse() const
{
	return _sizeaddr;
}

Server const & Socket::getServer() const
{
	return _server;
}
