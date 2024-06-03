#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpField.class.hpp"
# include "HttpRequestLine.class.hpp"
# include "Shared.hpp"
# include "Socket.class.hpp"
# include "HttpResponseStatus.hpp"
# include "HttpResponse.class.hpp"
# include "utils.hpp"
# include "Cgi.class.hpp"

# include <vector>

#define READ_SIZE 5
typedef enum{
	GET,
	POST,
	DELETE,
	NONE
} e_http_method;

class HttpRequest : public HttpRequestLine
{
	public:
		HttpRequest( void );
		virtual ~HttpRequest( void );
		HttpRequest ( HttpRequest const & model );
		virtual HttpRequest & operator=(HttpRequest const & model);

		virtual void	init( std::stringstream &request_stream ) throw(ExceptionHttpStatusCode);
		virtual void	displayRequest( void ) const;

		virtual void	processHeader( Socket const * const socket ) = 0;
		virtual void	generateResponse( Socket const * const socket, HttpResponse &response ) = 0;
		virtual void	readBody(int fd, Socket const * const socket, bool &end) = 0;
		virtual bool	hasBody() const = 0;
		virtual bool	hasCgi() const = 0;
		virtual void	setCgi(bool has_cgi) = 0;
		virtual Cgi		*getCgi() = 0;
		virtual std::string	getUri(std::string root);
		

		void				addStringToBody( std::string const & extra_body_content);
		std::string	const &	getBody( void ) const;

		bool							checkFieldExistence(std::string const & field_name) const;
		std::vector<std::string> const	&getFieldValue(std::string const & field_name) const throw(ExceptionHttpStatusCode);

		bool				isAcceptedMethod(Location const & location) const;

	protected:
		std::string	_body;
		
	private:
		std::vector<HttpField> _fields;
};

#endif