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

# include <vector>

#define READ_SIZE 100
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
		virtual void init( std::stringstream &request_stream ) throw(ExceptionHttpStatusCode);
		virtual ~HttpRequest( void );

		HttpRequest ( HttpRequest const & model );
		virtual HttpRequest & operator=(HttpRequest const & model);
		
		virtual void					processHeader( Socket const * const socket ) = 0;
		virtual void					generateResponse( Socket const * const socket, HttpResponse &response ) = 0;
		virtual void					readBody(int fd, Socket const * const socket, bool &end) = 0;
		virtual bool					hasBody() const = 0;
		
		std::string	const &				getBody( void ) const;

		void							addStringToBody( std::string const & extra_body_content);

		bool							checkFieldExistence(std::string const & field_name) const;
		const std::vector<std::string>	&getFieldValue(std::string const & field_name) const throw(ExceptionHttpStatusCode);

		bool							isAcceptedMethod(Location const & location) const;
		virtual void					displayRequest( void ) const;
	protected:
		std::string	_body;
	private:
		std::vector<HttpField> _fields;

		void	_fill_fields(std::stringstream &request_stream) throw (ExceptionHttpStatusCode);    
};

#endif