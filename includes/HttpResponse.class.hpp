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


		void		setStatusCode(e_status_code code);
		void		setVersion(it_version version);
		void		setBody(std::string &body_content);
		void		setEndOfFile();
		void		addAllowMethod(std::vector<std::string> const &);
		void		addField(std::string name, std::string value);
		void		addBodyContent(std::string str);
		void		removeBody();
		std::string const &getBody(void) const;
		bool		is_response_ready() const;

		bool							checkFieldExistence(std::string const & field_name) const;
		std::vector<std::string> const &getFieldValue(std::string const & field_name)
								const throw(ExceptionHttpStatusCode);

		void			writeResponse(int fd, Cluster &cluster, bool has_cgi);

		void			generateErrorResponse(e_status_code status, Server const & server, it_version method);
		void			parseCgiHeader(std::string header) throw(ExceptionHttpStatusCode);
		bool			handle_redirect(Location const &);
		void			fillHeader(void);
		void			displayHeader(void);
		e_status_code	openBodyFileStream(std::string filename);
		void			closeBodyFile(void);

	private:
		ssize_t		_sendBodyString(int fd, bool has_cgi);
		void		_chunkResponse();
		bool		_checkEndCgi(bool has_cgi) const;
		void		_generateErrorPageBody(e_status_code error_code);
		void		_removeField(std::string const &);
		ssize_t		_sendBodyFile(int fd);
		
		void		_extract_cgi_fields_data( void );

		it_version				_version;
		e_status_code			_status_code;
		std::string				_custom_status;

		std::vector<HttpField>	_fields;
		std::string				_header;

		std::string				_body;
		uint64_t				_content_length;
		bool					_content_length_flag;
		bool					_end_of_file_flag;
		uint64_t				_write_size;

		std::ifstream			_bodyFile;
		bool					_fileOpen;

		bool					_response_ready;
};

#endif