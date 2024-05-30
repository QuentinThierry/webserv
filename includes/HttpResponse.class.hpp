#ifndef HTTPREPONSE_CLASS_HPP
# define HTTPREPONSE_CLASS_HPP

# include "Shared.hpp"
# include "utils.hpp"
# include "Socket.class.hpp"
# include "HttpResponseStatus.hpp"
# include "HttpField.class.hpp"

# include <vector>
# include <sys/types.h>
# include <sys/stat.h>

#define SIZE_WRITE 100

class Cluster;

# define SERVER_NAME "webserv"

class HttpResponse
{
	public:
		HttpResponse( HttpResponse const & model);
		HttpResponse( it_version const & version);
		~HttpResponse( void );
		HttpResponse( void );
		HttpResponse & operator=(HttpResponse const & model );

		void		parseCgiHeader(std::string header) throw(ExceptionHttpStatusCode);

		bool		handle_redirect(Location const &);
		void		fillHeader();

		void		setStatusCode(e_status_code code);
		void		setVersion(it_version version);
		void		addAllowMethod(std::vector<std::string> const &);
		void		addField(std::string name, std::string value);
		void		addBodyContent(std::string str);

		e_status_code	openBodyFileStream(std::string filename);
		bool			checkFieldExistence(std::string const & field_name) const;
		std::vector<std::string> const &getFieldValue(std::string const & field_name) const throw(ExceptionHttpStatusCode);

		void		generateErrorResponse(e_status_code status, Server const & server);

		void		writeResponse(int fd, Cluster &cluster);
		void		displayHeader();
		bool		is_response_ready();

	private:
		void	_extract_cgi_fields_data( void );

		it_version				_version;
		e_status_code			_status_code;

		std::vector<HttpField>	_fields;
	
		std::string				_header;

		std::string				_body;
		uint64_t				_content_length;
		uint64_t				_read_size;

		std::ifstream			_bodyFile;
		bool					_fileOpen;

		bool					_response_ready;
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
 */

#endif