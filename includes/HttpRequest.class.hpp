/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:27:23 by acardona          #+#    #+#             */
/*   Updated: 2024/05/03 22:23:00 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpField.class.hpp"
# include "HttpRequestLine.class.hpp"

class HttpResponse;

class HttpRequest : public HttpRequestLine
{
	public:
		HttpRequest ( HttpRequest const & model );
		virtual HttpRequest & operator=(HttpRequest const & model);
		
		virtual void					process_header( void ) = 0;
		virtual HttpResponse			generate_response( void ) = 0;
		virtual void					display_request( void ) const;

		HttpRequest ( void );
		virtual void init( std::stringstream &request_stream ) throw(ExceptionHttpStatusCode);
		virtual ~HttpRequest( void );

	protected:

		bool							checkFieldExistence(std::string const & field_name) const;
		const std::vector<std::string>	&getFieldValue(std::string const & field_name) const throw(ExceptionHttpStatusCode);


	private:

		std::vector<HttpField> _fields;
		void	_fill_fields(std::stringstream &request_stream) throw (ExceptionHttpStatusCode);	
};

#endif