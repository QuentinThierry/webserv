#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

# include "Server.class.hpp"
# include "Shared.hpp"

# include <sys/socket.h> //socket(), bind(), listen(), accpet()
# include <netinet/in.h> //sockaddr_in
# include <arpa/inet.h> // htonl() htons()
# include <unistd.h>
# include <string>
# include <cstdio>
# include <cstring>
# include <cerrno>


# define NB_QUEUE_PORT 50

class Socket{
	public:
		Socket(Server const &server);
		Socket(Server const &server, Socket const & socket);
		Socket(Socket const &copy);
		Socket &operator=(Socket const &copy);
		~Socket();
		int const & 				getFd() const;
		struct sockaddr_in const &	getAddress() const;
		socklen_t const &			getSizeAddress() const;
		Server const &				getServer() const;
	private:
		Socket();
		int						_fd;
		struct sockaddr_in		_addr;
		socklen_t				_sizeaddr;
		Server const			_server;
};

#endif
