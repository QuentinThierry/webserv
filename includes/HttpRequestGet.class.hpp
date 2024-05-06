/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestGet.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:23:29 by acardona          #+#    #+#             */
/*   Updated: 2024/05/06 17:56:01 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTGET_CLASS_HPP
# define HTTPREQUESTGET_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpRequest.class.hpp"

# define MSG_ERR_HTTPGET_SSTREAM_FAIL "ERROR: internal: stringstream failure in the get request construction"
# define MSG_ERR_HTTPGET_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (GET)"

class HttpResponse;

class HttpRequestGet : public HttpRequest
{
	public:
		HttpRequestGet (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestGet ( HttpRequestGet const & model);
		HttpRequestGet & operator= (HttpRequestGet const & model);
		~HttpRequestGet( void );
		

		void				process_header( void );
		HttpResponse		generate_response( void );
		bool				hasBody() const;

	private:
		HttpRequestGet( void );
};

# endif