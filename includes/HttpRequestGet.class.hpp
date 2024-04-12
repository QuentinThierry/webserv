/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestGet.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:23:29 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 19:28:51 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTGET_CLASS_HPP
# define HTTPREQUESTGET_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpRequest.class.hpp"

class HttpRequestGet : public HttpRequest
{
	public:
		HttpRequestGet (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestGet ( HttpRequestGet const & model);
		HttpRequestGet & operator= (HttpRequestGet const & model);

	private:
		HttpRequestGet( void );
		~HttpRequestGet( void );

};

# endif