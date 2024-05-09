/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTools.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:43:37 by acardona          #+#    #+#             */
/*   Updated: 2024/05/09 12:08:05 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPTOOLS_HPP
# define HTTPTOOLS_HPP

# include "Shared.hpp"
# include "HttpResponseStatus.hpp"

# include <vector>
# include <algorithm>

# define HTTP_SEPARATORS "()<>@,;:\\/[]?={} \""
# define HTTP_WHITESPACES " \t"

static std::vector<std::string> g_http_methods;
typedef std::vector<std::string>::const_iterator it_method;
static std::vector<std::string> g_http_versions;
typedef std::vector<std::string>::const_iterator it_version;

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

e_status	find_end_word(std::string const & str, size_t start_idx,
				std::string delimiters, size_t &end_idx);

void		throw_http_err_with_log(e_status_code error_code, std::string log_msg);

#endif