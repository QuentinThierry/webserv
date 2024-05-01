#ifndef HTTPEXCHANGE_CLASS_HPP
# define HTTPEXCHANGE_CLASS_HPP

# include "Socket.class.hpp"
# include "HttpRequestLine.class.hpp"
class Cluster;

class Get;
class Post;
class Delete;

typedef union u_http_method{
	Get method_get;
	Post method_post;
	Delete method_delete;
} u_http_method;

typedef enum e_http_method{
	GET,
	POST,
	DELETE,
	NONE
} e_http_method;

class HttpExchange{
	public:
		HttpExchange(Socket const &);
		void readSocket(int fd, Cluster &cluster);
		void writeSocket(int fd, Cluster &cluster);
	private:
		Socket const		*_socket;
		std::string			_buffer_read;
		int64_t				_size_read;
		int64_t				_size_chunk;
		e_http_method		_method;
		u_http_method		_request;
		HttpExchange();
		void _setMethod();
		void _init_request();
		void _setRightSocket(Cluster const &cluster);
		void _handle_header(int fd, Cluster &cluster);
		void _handle_body(int fd, Cluster &cluster)


};

#endif