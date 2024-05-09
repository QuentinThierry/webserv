#ifndef HTTPEXCHANGE_CLASS_HPP
# define HTTPEXCHANGE_CLASS_HPP

# include "Socket.class.hpp"
#include "HttpRequest.class.hpp"
#include "HttpResponse.class.hpp"

class Cluster;

class HttpExchange{
	public:
		HttpExchange(Socket const &);
		HttpExchange(HttpExchange const &);
		HttpExchange & operator=(HttpExchange const &);
		~HttpExchange();
		void readSocket(int fd, Cluster &cluster);
		void writeSocket(int fd, Cluster &cluster);
	private:
		HttpExchange();
		Socket const		*_socket;
		std::string			_buffer_read;
		HttpRequest			*_request;
		HttpResponse		_response;

		void _initRequest(e_http_method method);
		void _copyRequest(e_http_method method, HttpRequest const * request);
		e_http_method _findMethod(std::string const & cmp);
		void _setRightSocket(Cluster const &cluster);
		void _handleHeader(int fd, Cluster &cluster);
		void _handleError(int fd, Cluster &cluster, e_status_code error);
};

#endif