/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:47:57 by acardona          #+#    #+#             */
/*   Updated: 2024/04/04 19:58:16 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpTools.hpp"

static bool _str_contain_a_colon(std::string const & str)
{
	return (str.find(':') != std::string::npos);
}

static bool _str_contains_multiple_colon(std::string const & str)
{
	return (_str_contain_a_colon(str) && str.find(':') != str.find_last_of(":") );
}

bool str_contain_one_single_colon(std::string const & str)
{
	return (_str_contain_a_colon(str) && ! _str_contains_multiple_colon(str));
}
