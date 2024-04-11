#ifndef HTTPEXCHANGE_CLASS_HPP
# define HTTPEXCHANGE_CLASS_HPP

# include "Socket.class.hpp"
# include "HttpRequestLine.class.hpp"
class Cluster;

class HttpExchange{
	private:
		Socket const		*_socket;
		std::string			_buffer_read;
		HttpRequestLine		_request;
		HttpExchange();
	public:
		HttpExchange(Socket const &);
		void readSocket(int fd, Cluster &cluster);
		void setRightSocket(Cluster const &cluster);
};

#endif