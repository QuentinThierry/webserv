#include "HttpTools.hpp"

static void	_init_available_http_methods( void )
{
	g_http_methods.push_back("HEAD");
	g_http_methods.push_back("GET");
	g_http_methods.push_back("POST");
	g_http_methods.push_back("DELETE");
}

static void	_init_available_http_versions( void )
{
	g_http_versions.push_back("HTTP");
	g_http_versions.push_back("HTTP/0.9");
	g_http_versions.push_back("HTTP/1.0");
	g_http_versions.push_back("HTTP/1.1");
}


static void _add_one_content_type(std::string extension, std::string mime_type)
{
	g_http_content_type.insert(std::pair<std::string, std::string>(extension, mime_type));
}

static void	_init_http_content_type( void )
{
	_add_one_content_type("default", "application/octet-stream");
	_add_one_content_type(".aac", "audio/aac");
	_add_one_content_type(".abw", "application/x-abiword");
	_add_one_content_type(".apng", "image/apng");
	_add_one_content_type(".arc", "application/x-freearc");
	_add_one_content_type(".avif", "image/avif");
	_add_one_content_type(".avi", "video/x-msvideo");
	_add_one_content_type(".azw", "application/vnd.amazon.ebook");
	_add_one_content_type(".bin", "application/octet-stream");
	_add_one_content_type(".bmp", "image/bmp");
	_add_one_content_type(".bz", "application/x-bzip");
	_add_one_content_type(".bz2", "application/x-bzip2");
	_add_one_content_type(".cda", "application/x-cdf");
	_add_one_content_type(".csh", "application/x-csh");
	_add_one_content_type(".css", "text/css; charset=utf-8");
	_add_one_content_type(".csv", "text/csv");
	_add_one_content_type(".doc", "application/msword");
	_add_one_content_type(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	_add_one_content_type(".eot", "application/vnd.ms-fontobject");
	_add_one_content_type(".epub", "application/epub+zip");
	_add_one_content_type(".gz", "application/gzip");
	_add_one_content_type(".gif", "image/gif");
	_add_one_content_type(".htm", "text/html; charset=utf-8");
	_add_one_content_type(".html", "text/html; charset=utf-8");
	_add_one_content_type(".ico", "image/vnd.microsoft.icon");
	_add_one_content_type(".ics", "text/calendar");
	_add_one_content_type(".jar", "application/java-archive");
	_add_one_content_type(".jpeg, .jpg", "image/jpeg");
	_add_one_content_type(".js", "text/javascript");
	_add_one_content_type(".json", "application/json");
	_add_one_content_type(".jsonld", "application/ld+json");
	_add_one_content_type(".mid, .midi", "audio/midi, audio/x-midi");
	_add_one_content_type(".mjs", "text/javascript");
	_add_one_content_type(".mp3", "audio/mpeg");
	_add_one_content_type(".mp4", "video/mp4");
	_add_one_content_type(".mpeg", "video/mpeg");
	_add_one_content_type(".mpkg", "application/vnd.apple.installer+xml");
	_add_one_content_type(".odp", "application/vnd.oasis.opendocument.presentation");
	_add_one_content_type(".ods", "application/vnd.oasis.opendocument.spreadsheet");
	_add_one_content_type(".odt", "application/vnd.oasis.opendocument.text");
	_add_one_content_type(".oga", "audio/ogg");
	_add_one_content_type(".ogv", "video/ogg");
	_add_one_content_type(".ogx", "application/ogg");
	_add_one_content_type(".opus", "audio/opus");
	_add_one_content_type(".otf", "font/otf");
	_add_one_content_type(".png", "image/png");
	_add_one_content_type(".pdf", "application/pdf");
	_add_one_content_type(".php", "application/x-httpd-php");
	_add_one_content_type(".ppt", "application/vnd.ms-powerpoint");
	_add_one_content_type(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
	_add_one_content_type(".rar", "application/vnd.rar");
	_add_one_content_type(".rtf", "application/rtf");
	_add_one_content_type(".sh", "application/x-sh");
	_add_one_content_type(".svg", "image/svg+xml");
	_add_one_content_type(".tar", "application/x-tar");
	_add_one_content_type(".tif, .tiff", "image/tiff");
	_add_one_content_type(".ts", "video/mp2t");
	_add_one_content_type(".ttf", "font/ttf");
	_add_one_content_type(".txt", "text/plain; charset=utf-8");
	_add_one_content_type(".vsd", "application/vnd.visio");
	_add_one_content_type(".wav", "audio/wav");
	_add_one_content_type(".weba", "audio/webm");
	_add_one_content_type(".webm", "video/webm");
	_add_one_content_type(".webp", "image/webp");
	_add_one_content_type(".woff", "font/woff");
	_add_one_content_type(".woff2", "font/woff2");
	_add_one_content_type(".xhtml", "application/xhtml+xml");
	_add_one_content_type(".xls", "application/vnd.ms-excel");
	_add_one_content_type(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	_add_one_content_type(".xml", "application/xml");
	_add_one_content_type(".xul", "application/vnd.mozilla.xul+xml");
	_add_one_content_type(".zip", "application/zip");
	_add_one_content_type(".3gp", "video/3gpp");
	_add_one_content_type(".3g2", "video/3gpp2");
	_add_one_content_type(".7z", "application/x-7z-compressed");
}

void	_init_available_http_methods_versions_type( void )
{
	_init_available_http_methods();
	_init_available_http_versions();
	_init_http_content_type();
}

std::string get_MIME_type(std::string const &file_uri)
{
	size_t		extension_start;


	extension_start = file_uri.find_last_of(".");
	if (extension_start != std::string::npos)
	{
		try
		{
			return (g_http_content_type.at(file_uri.substr(extension_start)));
		}
		catch (std::out_of_range &e)
		{
		}
	}
	return (g_http_content_type.at("default"));
}


bool	str_contains_a_colon(std::string const & str, size_t first_colon_pos)
{
	if (first_colon_pos == std::string::npos)
		first_colon_pos = str.find(':');
	return (first_colon_pos != std::string::npos);
}


//after a getline, if the line ended with \n it must be preceded by a \r
bool	is_line_properly_ended(std::stringstream & stream, std::string & line)
{
	if (stream.eof())
		return (true);
	if (!line.size() || line.at(line.size() - 1) != '\r')
		return (false);
	line.erase(line.end() - 1);
	return (true);
}


bool	is_whitespace(const unsigned char c)
{
	std::string whitespaces(HTTP_WHITESPACES);

	return (whitespaces.find(c) != std::string::npos);
}

/* === trim whitespaces (space and htab) === */

void	trim_whitespace(std::string & str)
{
    trim_whitespace_r(str);
    trim_whitespace_l(str);
}

void	trim_whitespace_l(std::string & str)
{
	size_t	whitespace_pos = str.find_first_not_of(HTTP_WHITESPACES);
	if (whitespace_pos == std::string::npos)
		str.clear();
	else if (whitespace_pos != 0)
    	str.erase(0, whitespace_pos);
}

void	trim_whitespace_r(std::string & str)
{
	size_t	whitespace_pos = str.find_last_not_of(HTTP_WHITESPACES);
	if (whitespace_pos == std::string::npos)
		str.clear();
	else if (whitespace_pos != str.size() - 1)
    	str.erase(whitespace_pos + 1);
}


/* ==== ABNF ====*/
// https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1

bool	is_ascii(unsigned char const &c)
{
	return (c <= 128);
}

bool	is_VCHAR(unsigned char const &c)
{
	return (c >= 0x21 && c <= 0x7E);
}

bool	is_tchar(unsigned char const &c)
{
	return (is_VCHAR(c) && !is_separator(c));
}

bool	is_control(unsigned char const &c)
{
	return (c <= 31 || c == 127);
}

bool	is_separator(unsigned char const &c)
{
	return (static_cast<std::string>(HTTP_SEPARATORS).find(c) != std::string::npos);
}


//set the first letter of the words (consecutive letters) in upper case and its other letters in lower case 
void	format_string_to_canonical(std::string & str)
{
	bool	to_capitalize = true;

	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (std::isalpha(*it))
		{
			if (to_capitalize)
			{
				*it = std::toupper(*it);
				to_capitalize = false;
			}
			else
			{
				*it = std::tolower(*it);
			}
		}
		else
		{
			to_capitalize = true;
		}
	}
}

bool	is_escaped(std::string const & str, size_t index)
{
	int backslash_cpt = 0;

	while (index - backslash_cpt > 0 && str.at(index - backslash_cpt - 1) == '\\')
		++backslash_cpt;
	return (backslash_cpt % 2);
}

static e_status	_get_end_quote_idx(std::string const & str,
					size_t const start_word_idx, size_t & end_word_idx)
{
	if (start_word_idx < str.length() - 1)
		end_word_idx = str.find('"', start_word_idx + 1);
	else
		end_word_idx = std::string::npos;
	if (end_word_idx == std::string::npos)
		return (FAILURE);
	else if (is_escaped(str, end_word_idx))
		return (_get_end_quote_idx(str, end_word_idx, end_word_idx));
	return (SUCCESS);
}

// sets end_index to point to the first char of str after the word (set of char
// ended by delimiters) starting at start_idx. Delimiters are ignored if escaped
// or quoted.
e_status find_end_word(std::string const & str, size_t start_idx,
				std::string delimiters, size_t &end_idx)
{
	std::string	special_char = delimiters.append("\"");

	if (start_idx >= str.size())
	{
		end_idx = str.size();
		return (SUCCESS);
	}
	end_idx = str.find_first_of(special_char, start_idx);
	if ( end_idx == std::string::npos )
	{
		end_idx = str.size();
		return (SUCCESS);
	}
	else if (str[end_idx] == '"')
	{
		if (is_escaped(str, end_idx))
			return (find_end_word(str, end_idx + 1, delimiters, end_idx));
		if ( _get_end_quote_idx(str, end_idx, end_idx) == FAILURE )
			return (FAILURE);
		return (find_end_word(str, end_idx + 1, delimiters, end_idx));
	}
	else // str[end_idx] == a delimiter
	{
		if (is_escaped(str, end_idx))
			return (find_end_word(str, end_idx + 1, delimiters, end_idx));
		return (SUCCESS);
	}
}

void	throw_http_err_with_log(e_status_code error_code, std::string log_msg)
{
	protected_write(g_err_log_fd, log_msg);
	throw(ExceptionHttpStatusCode(error_code));
}
