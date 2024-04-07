#ifndef CLIENT_CLASS_HPP
# define CLIENT_CLASS_HPP
# include "Server.class.hpp"
class Server; // need to replace by #include "Server.hpp"

# define READ 0
# define WRITE 1


class Client{
	private:
		int _statut;
		int _fd;
		struct sockaddr_in _addr;
		socklen_t _sizeaddr
	public:


};

#endif
