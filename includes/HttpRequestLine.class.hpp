/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:32:04 by acardona          #+#    #+#             */
/*   Updated: 2024/05/02 18:35:03 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTLINE_CLASS_HPP
# define HTTPREQUESTLINE_CLASS_HPP

# include "Shared.hpp"
# include "HttpTools.hpp"
# include "HttpField.class.hpp"

class HttpRequestLine
{
	public:
		HttpRequestLine( std::string const & request_header ) throw (ExceptionHttpStatusCode);
		HttpRequestLine( HttpRequestLine const & model);
		HttpRequestLine & operator=(HttpRequestLine const & model );

		it_method	const & getMethod( void ) const;
		std::string	const & getTarget( void ) const;
		it_version	const & getVersion( void ) const;


	private:
		HttpRequestLine( void );
		it_method		_method;
		std::string		_target;
		it_version		_version;
};

#endif