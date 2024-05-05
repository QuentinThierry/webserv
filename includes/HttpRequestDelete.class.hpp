/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestDelete.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:23:29 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 19:28:51 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTDELETE_CLASS_HPP
# define HTTPREQUESTDELETE_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpRequest.class.hpp"

# define MSG_ERR_HTTPDELETE_SSTREAM_FAIL "ERROR: internal: stringstream failure in the delete request construction"
# define MSG_ERR_HTTPDELETE_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (DELETE)"

class HttpResponse;

class HttpRequestDelete : public HttpRequest
{
	public:
		HttpRequestDelete (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestDelete ( HttpRequestDelete const & model);
		HttpRequestDelete & operator= (HttpRequestDelete const & model);
		~HttpRequestDelete( void );

		void			process_header( void );
		HttpResponse	generate_response( void );

	private:
		HttpRequestDelete( void );
};
# endif