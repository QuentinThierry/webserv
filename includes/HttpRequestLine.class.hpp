/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:32:04 by acardona          #+#    #+#             */
/*   Updated: 2024/05/03 22:04:48 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTLINE_CLASS_HPP
# define HTTPREQUESTLINE_CLASS_HPP

# include "Shared.hpp"
# include "HttpTools.hpp"
# include "HttpField.class.hpp"

# define MSG_ERR_LINE_MISSING_WORD "ERROR: Http request: incomplete request first line"
# define MSG_ERR_LINE_WRONG_METHOD "ERROR: Http request: method incorrect or not supported"
# define MSG_ERR_LINE_WRONG_VERSION "ERROR: Http request: version incorrect or not supported"
# define MSG_ERR_LINE_TOO_MANY_WORDS "ERROR: Http request: too much words in the request first line"
# define MSG_ERR_LINE_SSTREAM_FAIL "ERROR: internal: stringstream failure in the request line parsing"
# define MSG_ERR_LINE_BAD_SSTREAM_INPUT "ERROR: internal: stringstream input corrupted in the request line parsing"

class HttpRequestLine
{
	public:
		HttpRequestLine( HttpRequestLine const & model);
		HttpRequestLine & operator=(HttpRequestLine const & model );

		it_method const &	getMethod( void ) const;
		std::string const &	getTarget( void ) const;
		it_version const &	getVersion( void ) const;

		void	display_request_line( void ) const;

	protected :
		HttpRequestLine( void );
		void	_fill_request_line( std::string const & request_first_line ) throw (ExceptionHttpStatusCode);
		void	_fill_request_line_with_stream( std::stringstream & request_header_stream ) throw (ExceptionHttpStatusCode);

		//the following constructors should not be necessary but I let them until we are sure about their uselessness
		HttpRequestLine( std::stringstream & request_first_line ) throw (ExceptionHttpStatusCode);
		HttpRequestLine( std::string const & request_first_line ) throw (ExceptionHttpStatusCode);

	private:
		it_method		_method;
		std::string		_target;
		it_version		_version;
};

#endif