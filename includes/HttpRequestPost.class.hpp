/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestPost.class.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:23:29 by acardona          #+#    #+#             */
/*   Updated: 2024/05/05 19:44:54 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPOST_CLASS_HPP
# define HTTPREQUESTPOST_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpRequest.class.hpp"

# define MSG_ERR_HTTPPOST_SSTREAM_FAIL "ERROR: internal: stringstream failure in the post request construction"
# define MSG_ERR_HTTPPOST_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (POST)"


class HttpRequestPost : public HttpRequest
{
	public:
		HttpRequestPost (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestPost ( HttpRequestPost const & model);
		HttpRequestPost & operator= (HttpRequestPost const & model);
		~HttpRequestPost( void );

		std::string	const &	getBody( void ) const;
		void				addStringToBody( std::string const & extra_body_content);

		void	display_request( void ) const;
	
		void			process_header( void );
		HttpResponse	generate_response( void );

	private:
		HttpRequestPost( void );
		std::string _body;
};

# endif