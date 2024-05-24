#ifndef AUTOINDEX_CLASS_HPP
# define AUTOINDEX_CLASS_HPP

# include "Shared.hpp"
# include "HttpResponseStatus.hpp"

# include <vector>
# include <algorithm>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>

#define HAS_READ_RIGHT(mode) (((mode) & (1 << 8)) != 0)

typedef enum
{
	IS_DIRECTORY,
	IS_FILE
}	document_type;

typedef struct 
{
	std::string		name;
	document_type	type;
	size_t			size;
	time_t			last_modified;
	bool			is_reachable;

} s_document_data;

class Autoindex
{
	public:
		Autoindex( std::string uri )  throw (ExceptionHttpStatusCode);
		void	generateAutoIndexBody(std::string &response_body);

		std::vector<s_document_data>	& GetDocumentsData();//

	private:
		Autoindex(void);
		Autoindex(Autoindex const &model);
		Autoindex & operator=(Autoindex const & model);

		std::vector<s_document_data>	_documents_data;
		std::string 					_uri_root;

		e_status	addDocument(dirent *doc);
		std::string	_generate_html_header( void );
		std::string	_generate_html_body(std::string &uri);

};

#endif