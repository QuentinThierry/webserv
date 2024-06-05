#ifndef HTTPREQUESTHEAD_CLASS_HPP
# define HTTPREQUESTHEAD_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "HttpTools.hpp"
# include "Autoindex.class.hpp"

# include <sys/stat.h>

# define MSG_ERR_HTTPHEAD_SSTREAM_FAIL "ERROR: internal: stringstream failure in the get request construction"
# define MSG_ERR_HTTPHEAD_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (HEAD)"

class HttpResponse;

class HttpRequestHead : public HttpRequest
{
	public:
		HttpRequestHead (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestHead ( HttpRequestHead const & model);
		HttpRequestHead & operator= (HttpRequestHead const & model);
		~HttpRequestHead( void );

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
		HttpRequestHead( void );

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