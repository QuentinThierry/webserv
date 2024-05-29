#ifndef HTTPREQUESTGET_CLASS_HPP
# define HTTPREQUESTGET_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "HttpTools.hpp"

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
		bool	hasCgi() const;
		Cgi	*getCgi();

	private:
		void	_initResponse( Socket const * const socket, HttpResponse &response );
		void	_handleDirectory(std::string & uri, Location const & location, HttpResponse & response, Server const &server);
		void	_redirectDirectory(HttpResponse & response);
		void	_handle_file(std::string & uri, HttpResponse & response, Server const & server);
		e_status	_handle_index_file(std::string & uri, Location const & location,
				HttpResponse & response,  Server const & server);
		void	_handleCgi(std::string & uri, Server const & server, CgiLocation const &cgi_location);

		HttpRequestGet( void );

		Cgi		_cgi;
		bool	_has_cgi;
};

# endif