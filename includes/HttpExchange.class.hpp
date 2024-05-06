#ifndef HTTPEXCHANGE_CLASS_HPP
# define HTTPEXCHANGE_CLASS_HPP

# include "Socket.class.hpp"
# include "HttpRequestPost.hpp"

class Cluster;

class HttpExchange{
	public:
		HttpExchange(Socket const &);
		void readSocket(int fd, Cluster &cluster);
		void writeSocket(int fd, Cluster &cluster);
	private:
		Socket const		*_socket;
		std::string			_buffer_read;
		HttpRequest			*_request;

		HttpExchange();
		void _initRequest(e_http_method method);
		e_http_method _findMethod();
		void _setRightSocket(Cluster const &cluster);
		void _handleHeader(int fd, Cluster &cluster);
		void _handleBody(int fd, Cluster &cluster);
};

#endif