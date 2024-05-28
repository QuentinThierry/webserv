#ifndef AUTOINDEX_CLASS_HPP
# define AUTOINDEX_CLASS_HPP

# include "Shared.hpp"
# include "HttpResponseStatus.hpp"

# include <vector>
# include <algorithm>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <ctime>

#define HAS_READ_RIGHT(mode) (((mode) & (1 << 8)) != 0)

#define MAX_WIDTH_NAME "40vw"
#define MAX_WIDTH_SIZE "20vw"
#define MAX_WIDTH_DATE "20vw"

#define PADDING_LEFT_NAME "1vw"
#define PADDING_LEFT_SIZE "3vw"
#define PADDING_LEFT_DATE "3vw"

typedef enum
{
	IS_DIRECTORY,
	IS_FILE
}	document_type;

typedef enum
{
	CELL_HEADER,
	CELL_NORMAL
}	e_cell_type;

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
		Autoindex( std::string const & location_root, std::string const & target )  throw (ExceptionHttpStatusCode);

		std::string	generateAutoIndexBody( void );

		std::vector<s_document_data>	& GetDocumentsData();//

	// private:
		Autoindex(void);
		Autoindex(Autoindex const &model);
		Autoindex & operator=(Autoindex const & model);

		std::vector<s_document_data>	_documents_data;
		std::string const				_location_root;
		std::string 					_target;
		std::string 					_uri_target;


		e_status	addDocument(dirent *doc);
		std::string	_generate_html_header( void ) ;
		std::string	_generate_html_body( void );

};

#endif