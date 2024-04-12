/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTools.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:43:37 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 22:10:06 by acardona         ###   ########.fr       */
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


bool		str_contains_one_single_colon(std::string const & str,
				size_t first_colon_pos = std::string::npos);

bool		is_ascii(unsigned char const &c);
bool		is_VCHAR(unsigned char const &c);
bool		is_tchar(unsigned char const &c);
bool		is_control(unsigned char const &c);
bool		is_separator(unsigned char const &c);
bool		is_escaped(std::string const & str, size_t index);


void		trim_whitespace(std::string & str);
void		trim_whitespace_r(std::string & str);
void		trim_whitespace_l(std::string & str);

void		format_string_to_canonical(std::string & str);

#endif