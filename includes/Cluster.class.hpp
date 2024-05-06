#ifndef CLUSTER_CLIENT_HPP
# define CLUSTER_CLIENT_HPP

# include "shared.hpp"
# include "Socket.class.hpp"
# include "HttpExchange.class.hpp"

# include <sys/time.h>
# include <vector>
# include <deque>
# include <algorithm>


typedef std::vector<Socket>::const_iterator	t_const_iter_sockets;
typedef std::vector<Socket>::iterator		t_iter_sockets;
typedef std::vector<Socket>					t_sockets;

typedef std::deque<std::pair<int, HttpExchange> >::const_iterator	t_const_iter_map_sockets;
typedef std::deque<std::pair<int, HttpExchange> >::iterator			t_iter_map_sockets;
typedef std::deque<std::pair<int, HttpExchange> >					t_map_sockets;

class Cluster
{
	public:
		Cluster(std::vector<Server>);
		~Cluster();
		void runServer();
		void closeConnection(int fd);
		Socket const *get_matching_socket(int fd, std::string server_name) const;
		void switchHttpExchangeToWrite(int fd);
	private:
		Cluster();
		Cluster(Cluster const &copy);
		Cluster & operator=(Cluster const &copy);
		void _addServer(Server const & server);
		Socket const * _find_same_config_server(Server const &server) const;
		void _init_set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds) const;
		void _print_set(fd_set *fds, std::string str) const;
		void _acceptNewConnection(Socket const & socket);

		std::vector<Socket>				_sockets; //fd that are listening
		std::deque<std::pair<int, HttpExchange> >	_map_sockets; //fd create with accept => read
		std::vector<int>				_fd_write; //fd create with accept => write
		int								_max_fd;
		bool							_close_connection;

};

#endif