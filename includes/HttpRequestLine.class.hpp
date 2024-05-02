/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:32:04 by acardona          #+#    #+#             */
/*   Updated: 2024/05/02 20:10:11 by acardona         ###   ########.fr       */
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
		HttpRequestLine( std::stringstream & request_first_line ) throw (ExceptionHttpStatusCode);
		HttpRequestLine( std::string const & request_first_line ) throw (ExceptionHttpStatusCode);
		HttpRequestLine( HttpRequestLine const & model);
		HttpRequestLine & operator=(HttpRequestLine const & model );

		it_method	const & getMethod( void ) const;
		std::string	const & getTarget( void ) const;
		it_version	const & getVersion( void ) const;

	protected :
		HttpRequestLine( void );
		void	_fill_request_line( std::string const & request_first_line ) throw (ExceptionHttpStatusCode);
	private:
		it_method		_method;
		std::string		_target;
		it_version		_version;
};

#endif