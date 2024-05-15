/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:47:57 by acardona          #+#    #+#             */
/*   Updated: 2024/05/15 16:59:27 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpTools.hpp"

static void	_init_available_http_methods( void )
{
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

void	_init_available_http_methods_versions( void )
{
	_init_available_http_methods();
	_init_available_http_versions();
}


bool	str_contains_a_colon(std::string const & str, size_t first_colon_pos)
{
	if (first_colon_pos == std::string::npos)
		first_colon_pos = str.find(':');
	return (first_colon_pos != std::string::npos);
}

static bool	_str_contains_multiple_colon(std::string const & str,
				size_t first_colon_pos = std::string::npos)
{
	if (first_colon_pos == std::string::npos)
		first_colon_pos = str.find(':');
	return (str_contains_a_colon(str, first_colon_pos) && first_colon_pos != str.find_last_of(":") );
}

bool	str_contains_one_single_colon(std::string const & str,
			size_t first_colon_pos)
{
	if (first_colon_pos == std::string::npos)
		first_colon_pos = str.find(':');
	return (str_contains_a_colon(str, first_colon_pos) && ! _str_contains_multiple_colon(str, first_colon_pos));
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
	return (c >= 0 && c <= 128);
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

/*

// tests

#include <vector>

static void _test__get_end_quote_idx( void );
static void _test_find_end_word( void );

int main()
{
	_test__get_end_quote_idx();
	_test_find_end_word();
}


static void _test__get_end_quote_idx( void )
{
	std::vector<std::string>  fields;

	fields.push_back("\"");
	fields.push_back("\"abcde");
	fields.push_back("\"abcde\"");
	fields.push_back("\"abcde\"fgh");
	fields.push_back("\"abcde\"\"fgh");
	fields.push_back("\"abcde\"");
	fields.push_back("\"abcde\\\"fgh\"");

	std::cout << "Test _get_end_quote_idx : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\" (size = " << it->size() << "):";
		size_t	next_quote;

		if (_get_end_quote_idx(*it, 0, next_quote) == SUCCESS)
			std::cout << " -> success: " << next_quote << " (" << it->at(next_quote) << ")";
		else
			std::cout << " => not valid";
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

static void _test_find_end_word( void )
{
	std::vector<std::string>	fields;
	size_t						end_value;

	fields.push_back("simple_correct");
	fields.push_back("simple_correct, with_next");
	fields.push_back("\"quoted_correct\"abc");
	fields.push_back("\"quoted_correct\"continued");
	fields.push_back("\"quoted_correct\", next");
	fields.push_back("  simple_correct_space  ");
	fields.push_back("  incorrect_quote\"  ");

	std::cout << "Test find_end_word : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\" (size = " << it->size() << "):" << std::endl;

		if (find_end_word(*it, 0, ",", end_value) == SUCCESS)
		{
			std::cout << " -> success: " << end_value;
			if (end_value < it->size())
				std::cout << " ('" << it->at(end_value) << "')" << std::endl;
			else
				std::cout << " (end of string)" << std::endl;
		}
		else
		{
			std::cout << " => not valid";
		}
	}

	std::cout << std::endl;
}
*/