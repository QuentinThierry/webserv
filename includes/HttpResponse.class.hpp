#ifndef HTTPREPONSE_CLASS_HPP
# define HTTPREPONSE_CLASS_HPP
# include "Shared.hpp"
# include "Socket.class.hpp"
# include "HttpResponseStatus.hpp"
# include "HttpField.class.hpp"
# include <vector>


class HttpResponse
{
	public:
		HttpResponse( HttpResponse const & model);
		HttpResponse( it_version const & version);
		HttpResponse & operator=(HttpResponse const & model );
		void		generateErrorResponse(e_status_code status);
		~HttpResponse( void );
		HttpResponse( void );

		bool	handle_redirect(Location const &);

		void setStatusCode(e_status_code code);

	private:
		it_version		_version;
		int				_status_code;

		std::vector<HttpField>	_fields;
		std::string				_body;
};

/* 
Since the 205 status code implies that no additional content will be
   provided, a server MUST NOT generate a payload in a 205 response.  In
   other words, a server MUST do one of the following for a 205
   response: a) indicate a zero-length body for the response by
   including a Content-Length header field with a value of 0; b)
   indicate a zero-length payload for the response by including a
   Transfer-Encoding header field with a value of chunked and a message
   body consisting of a single chunk of zero-length; or, c) close the
   connection immediately after sending the blank line terminating the
   header section.

405 => Allow header
408 => connection close
 */

#endif