#ifndef HTTPEXCHANGE_CLASS_HPP
# define HTTPEXCHANGE_CLASS_HPP

# include "Socket.class.hpp"
# include "HttpRequestLine.class.hpp"

class HttpExchange{
	private:
		Socket const		*_socket;
		std::string			_buffer_read;
		HttpRequestLine		_request;
		HttpExchange();
	public:
		HttpExchange(Socket const &);
		void read_header(int fd);
};

#endif