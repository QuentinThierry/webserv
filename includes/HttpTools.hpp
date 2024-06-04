#ifndef HTTPTOOLS_HPP
# define HTTPTOOLS_HPP

# include "Shared.hpp"
# include "HttpResponseStatus.hpp"

# include <map>
# include <algorithm>

# define HTTP_SEPARATORS "()<>@,;:\\/[]?={} \""
# define HTTP_WHITESPACES " \t"

# define MSG_ERR_WRONG_END_OF_LINE "Error: http request: line not properly ended"

extern	std::vector<std::string>							g_http_versions;
typedef	std::vector<std::string>::const_iterator			it_version;
extern	std::map<std::string, std::string>					g_http_content_type;
typedef	std::map<std::string, std::string>::const_iterator	it_content_type;

void		_init_available_http_methods_versions_type( void );

std::string	get_MIME_type(std::string const &file_uri);

bool		str_contains_a_colon(std::string const & str,
				size_t first_colon_pos = std::string::npos);
bool		is_line_properly_ended(std::stringstream & stream, std::string & line);

bool		is_ascii(unsigned char const &c);
bool		is_VCHAR(unsigned char const &c);
bool		is_tchar(unsigned char const &c);
bool		is_control(unsigned char const &c);
bool		is_separator(unsigned char const &c);
bool		is_escaped(std::string const & str, size_t index);

bool		is_whitespace(const unsigned char c);
void		trim_whitespace(std::string & str);
void		trim_whitespace_r(std::string & str);
void		trim_whitespace_l(std::string & str);

void		format_string_to_canonical(std::string & str);

e_status	find_end_word(std::string const & str, size_t start_idx,
				std::string delimiters, size_t &end_idx);

void		throw_http_err_with_log(e_status_code error_code, std::string log_msg);

#endif