/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:47:57 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 21:30:16 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpTools.hpp"

static bool	_str_contain_a_colon(std::string const & str,
				size_t first_colon_pos = std::string::npos)
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
	return (_str_contain_a_colon(str, first_colon_pos) && first_colon_pos != str.find_last_of(":") );
}

bool	str_contains_one_single_colon(std::string const & str,
			size_t first_colon_pos)
{
	if (first_colon_pos == std::string::npos)
		first_colon_pos = str.find(':');
	return (_str_contain_a_colon(str, first_colon_pos) && ! _str_contains_multiple_colon(str, first_colon_pos));
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
