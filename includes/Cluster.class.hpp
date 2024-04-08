#ifndef CLUSTER_CLIENT_HPP
# define CLUSTER_CLIENT_HPP

# include "shared.hpp"
# include "Socket.class.hpp"

# include <sys/time.h>
# include <vector>


typedef std::vector<Socket>::const_iterator	t_const_iter_servers;
typedef std::vector<Socket>::iterator		t_iter_servers;
typedef std::vector<Socket>					t_servers;

typedef std::map<int, Socket const *>::const_iterator	t_const_iter_map_socket;
typedef std::map<int, Socket const *>::iterator		t_iter_map_socket;
typedef std::map<int, Socket const *>					t_map_socket;

class Cluster
{
	private:
		// std::vector<Server>	_all_server;
		std::vector<Socket>				_servers; //fd that are listening
		std::map<int, Socket const *>	_map_socket; //fd create with accept => read + write
		int								_max_fd;
	public:
		// Cluster();
		Cluster(std::vector<Server>);
		// Cluster(Cluster const &copy);
		// ~Cluster();
		Socket const * find_same_config_server(Server const &server);
		void addServer(Server const & server);
		void init_set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds);
		void runServer();
		void print_set(fd_set *fds, std::string str);
		void acceptNewConnection(Socket const & socket);
};

#endif