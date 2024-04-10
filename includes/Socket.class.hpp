#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

# include "Server.class.hpp"

# include <sys/socket.h> //socket(), bind(), listen(), accpet()
# include <netinet/in.h> //sockaddr_in
# include <arpa/inet.h> // htonl() htons()
# include <map>
# include <unistd.h>
# include <string.h>
# include <cstdio>

# define NB_QUEUE_PORT 5

class Socket{
	private:
		int						_fd;
		struct sockaddr_in		_addr;
		socklen_t				_sizeaddr;
		Server 					_server;
	public:
		Socket(Server const &server);
		Socket(Server const &server, Socket const & socket);
		int const & getFd() const;
		struct sockaddr_in const & getAddresse() const;
		socklen_t const & getSizeAddresse() const;
		Server const & getServer() const;
};

#endif
