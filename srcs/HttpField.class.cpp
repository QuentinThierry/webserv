/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpField.class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:52:09 by acardona          #+#    #+#             */
/*   Updated: 2024/04/04 19:42:45 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpField.class.hpp"

HttpField::HttpField ( void ): _name(""), _values("")
{}

HttpField::HttpField( std::string const & whole_line ) throw (ExceptionHttpStatusCode)
{
	size_t separator = whole_line.find(":");
	if ( separator == std::string::npos
		|| separator == 0
		|| separator != whole_line.find_last_of(":")
		|| std::isspace(whole_line[separator - 1]))
		throw(ExceptionHttpStatusCode(HTTP_400));

}




HttpField::HttpField(std::string const & name, std::string const & values) : _name(name), _values(values)
{}

HttpField::HttpField(HttpField const & model) : _name(model._name), _values(model._values)
{}

HttpField::~HttpField( void )
{}

HttpField & HttpField::operator=(HttpField const & model)
{
	if (&model == this)
	{
		_name = model._name;
		_values=model._values;
	}
	return ( *this);
}


std::string const & HttpField::getName( void ) const
{
	return (_name);
}

std::string const & HttpField::getValues( void ) const
{
	return (_values);
}
