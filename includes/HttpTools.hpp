/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTools.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:43:37 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 19:17:59 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPTOOLS_HPP
# define HTTPTOOLS_HPP

# include "Shared.hpp"
# include "HttpResponseStatus.hpp"

# define HTTP_SEPARATORS "()<>@,;:\\/[]?={} \""
# define HTTP_WHITESPACES " \t"

typedef enum e_http_method
{
	GET,
	POST,
	DELETE
}	e_http_method;


bool	str_contains_one_single_colon(std::string const & str,
			size_t first_colon_pos = std::string::npos);

bool	is_control_char(unsigned char c);

bool	char_is_ALPHA(unsigned char const c);
bool	char_is_DIGIT(unsigned char const c);
bool	char_is_VCHAR(unsigned char const c);
bool	char_is_tchar(unsigned char const c);
bool	char_is_control(char const c);
bool	char_is_separator(char const c);

void	trim_whitespace(std::string & str);
void	trim_whitespace_r(std::string & str);
void	trim_whitespace_l(std::string & str);

void	format_string_to_canonical(std::string & str);

#endif