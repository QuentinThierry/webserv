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

class HttpRequestDelete : public HttpRequest
{
	public:
		HttpRequestDelete (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestDelete ( HttpRequestDelete const & model);
		HttpRequestDelete & operator= (HttpRequestDelete const & model);

	private:
		HttpRequestDelete( void );
		~HttpRequestDelete( void );

};

# endif