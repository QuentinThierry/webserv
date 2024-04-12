/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:32:04 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 19:17:16 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTLINE_CLASS_HPP
# define HTTPREQUESTLINE_CLASS_HPP

# include "Shared.hpp"
# include "HttpTools.hpp"
# include "HttpField.class.hpp"

class HttpRequestLine
{
		HttpRequestLine( std::string const & request_header ) throw (ExceptionHttpStatusCode);
		e_http_method const & getMethod( void ) const;
		std::string const & getTarget( void ) const;
		std::string const & getVersion( void ) const; 
	private:
		HttpRequestLine( void );
		e_http_method	_method;
		std::string		_target;
		std::string		_version;

};

#endif