#ifndef CLUSTER_CLIENT_HPP
# define CLUSTER_CLIENT_HPP

# include "Shared.hpp"
# include "Socket.class.hpp"
# include "HttpExchange.class.hpp"

# include <sys/time.h>
# include <vector>
# include <deque>
# include <algorithm>

# define TIMEOUT_SEC 30


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
		void			runServer();
		void			closeConnection(int fd);
		Socket const	*getMatchingSocket(int fd, std::string server_name) const;
		void			switchHttpExchangeToWrite(int fd);

	private:
		Cluster();
		Cluster(Cluster const &copy);
		Cluster & operator=(Cluster const &copy);
		void			_addServer(Server const & server);
		Socket const *	_findSameConfigServer(Server const &server) const;
		void			_initSetFds(fd_set *readfds, fd_set *writefds) const;
		void			_printSet(fd_set *fds, std::string str) const;
		void			_acceptNewConnection(Socket const & socket);
		void			_checkTimeout();

		std::vector<Socket>							_sockets; //fd that are listening
		std::deque<std::pair<int, HttpExchange> >	_map_sockets; //fd create with accept => read
		std::vector<int>							_fd_write; //fd create with accept => write
		int											_max_fd;
		bool										_close_connection;

};

#endif