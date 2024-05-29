#ifndef HTTPREQUESTGET_CLASS_HPP
# define HTTPREQUESTGET_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "HttpTools.hpp"

#include "Autoindex.class.hpp"

# define MSG_ERR_HTTPGET_SSTREAM_FAIL "ERROR: internal: stringstream failure in the get request construction"
# define MSG_ERR_HTTPGET_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (GET)"

class HttpResponse;

class HttpRequestGet : public HttpRequest
{
	public:
		HttpRequestGet (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestGet ( HttpRequestGet const & model);
		HttpRequestGet & operator= (HttpRequestGet const & model);
		~HttpRequestGet( void );
		

		void	processHeader( Socket const * const socket );
		void	generateResponse( Socket const * const socket, HttpResponse &response );
		bool	hasBody() const;
		void	readBody(int fd, Socket const * const socket, bool &end);

	private:
		void	_initResponse( Socket const * const socket, HttpResponse &response );
		void	_handleDirectory(std::string const & target, Location const & location, HttpResponse & response);
		void	_redirectDirectory(HttpResponse & response);

		HttpRequestGet( void );
};

# endif