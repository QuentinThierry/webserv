#ifndef SOCKETMANAGER_CLIENT_HPP
# define SOCKETMANAGER_CLIENT_HPP

# include "shared.hpp"
# include <sys/socket.h> //socket(), bind(), listen(), accpet()
# include <netinet/in.h> //sockaddr_in
# include <arpa/inet.h> // htonl() htons()
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <map>
# include <vector>

class Server; // need to replace by #include "Server.hpp"
# include "Client.class.hpp"

typedef typename std::vector<std::pair<int, Server const &> >::iterator iter_map_server;
typedef typename std::vector<std::pair<int, Server const &> > map_server;

typedef typename std::map<int, Client>::iterator iter_map_client;
typedef typename std::map<int, Client> map_client;

class SocketManager
{
	private:
		std::vector<Server> 							_all_server;
		std::vector<std::pair<int, Server const &> >	_map_server;
		std::map<int, Client>							_map_client;
		std::vector<int>								_server_fd;
		int												_max_fd = 0;
	public:
		SocketManager();
		SocketManager(SocketManager const &copy);
		~SocketManager();
		void addServer(Server const & server);
		void addClient(Client const & client);
		void init_set(fd_set *readfds, fd_set *writefds, fd_set *exceptfds);
		void runServer();
		void acceptNewConnection(std::pair<int, Server const &>  const & server);

};

int create_socket(uint16_t port, uint32_t host);

#endif

/*
Server{80, coucou}
Server{80, hey}
=> 2 server = 1 fd

Server{80, coucou} + accept



*/