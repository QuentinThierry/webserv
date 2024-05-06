#ifndef HTTPREQUESTPOST_HPP
# define HTTPREQUESTPOST_HPP
# include "HttpRequest.hpp"

class HttpRequestPost : public HttpRequest
{
	public:
		HttpRequestPost (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestPost ( HttpRequestPost const & model);
		HttpRequestPost & operator= (HttpRequestPost const & model);

		std::string	const &	getBody( void ) const;
		void				addStringToBody( std::string const & extra_body_content);
	private:
		HttpRequestPost( void );
		~HttpRequestPost( void );
		std::string _body;
};

#endif