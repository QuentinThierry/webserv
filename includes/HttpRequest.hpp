#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include "shared.hpp"
# include "Socket.class.hpp"
# include "HttpResponseStatus.hpp"
# include <vector>

#define READ_SIZE 20

typedef enum{
	GET,
	POST,
	DELETE,
	NONE
} e_http_method;

class HttpRequestLine
{
		HttpRequestLine( std::string const & request_header ) throw (ExceptionHttpStatusCode);
		HttpRequestLine( HttpRequestLine const & model);
		HttpRequestLine & operator=(HttpRequestLine const & model );

		e_http_method const & getMethod( void ) const;
		std::string const & getTarget( void ) const;
		std::string const & getVersion( void ) const;


	private:
		HttpRequestLine( void ){};


		e_http_method	_method;
		std::string		_target;
		std::string		_version;

};

class HttpRequest : public HttpRequestLine
{
	public:
		HttpRequest ( HttpRequest const & model );
		virtual HttpRequest & operator=(HttpRequest const & model);
		
		virtual void					process_header( Socket const * const socket ) = 0;
		// virtual HttpResponse			generate_response( void ) = 0;
		virtual void					readBody(int fd) = 0;
		virtual bool					hasBody() const = 0;

		virtual void					display_request( void ) const;

		HttpRequest ( void );
		virtual void init( std::stringstream &request_stream ) throw(ExceptionHttpStatusCode);
		virtual ~HttpRequest( void );

		bool							checkFieldExistence(std::string const & field_name) const;
		const std::vector<std::string>	&getFieldValue(std::string const & field_name) const throw(ExceptionHttpStatusCode);
		
		std::string	const &				getBody( void ) const;
		void							addStringToBody( std::string const & extra_body_content);
	private:
		// std::vector<HttpField> _fields;
		std::string	_body;

		void	_fill_fields(std::stringstream &request_stream) throw (ExceptionHttpStatusCode);    
};

#endif