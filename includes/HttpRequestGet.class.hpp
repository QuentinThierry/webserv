#ifndef HTTPREQUESTGET_CLASS_HPP
# define HTTPREQUESTGET_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "HttpTools.hpp"

# include "Autoindex.class.hpp"

# include <sys/stat.h>

# define MSG_ERR_HTTPGET_SSTREAM_FAIL "ERROR: internal: stringstream failure in the get request construction"
# define MSG_ERR_HTTPGET_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (GET)"

class HttpResponse;

class HttpRequestGet : public HttpRequest
{
	public:
		HttpRequestGet (std::string const & str_request, Cluster &cluster) throw (ExceptionHttpStatusCode);
		HttpRequestGet ( HttpRequestGet const & model);
		HttpRequestGet & operator= (HttpRequestGet const & model);
		~HttpRequestGet( void );

		void	processHeader( Socket const * const socket );
		void	generateResponse( Socket const * const socket, HttpResponse &response );
		bool	hasBody() const;
		void	readBody(int fd, Socket const * const socket, bool &end);
		bool	hasCgi() const;
		void	setCgi(bool has_cgi);
		Cgi		*getCgi();
		std::string	getUri(std::string root);
		std::string const &getQueryString() const;

	private:
		HttpRequestGet( void );

		void		_initResponse( Socket const * const socket, HttpResponse &response);
		void		_handleDirectory(std::string & uri, Location const & location,
							HttpResponse & response, Server const &server);
		void		_handleFile(std::string & uri, HttpResponse & response, Server const & server);
		e_status	_handle_index_file(std::string & uri, Location const & location,
							HttpResponse & response,  Server const & server);
		void		_handleCgi(Server const & server, CgiLocation const &cgi_location);
		void		_redirectDirectory(HttpResponse & response);

		Cgi			_cgi;
		bool		_has_cgi;
		std::string	_query_string;
};

# endif