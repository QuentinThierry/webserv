/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:27:23 by acardona          #+#    #+#             */
/*   Updated: 2024/04/12 19:23:04 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpField.class.hpp"
# include "HttpRequestLine.class.hpp"


class HttpRequest : public HttpRequestLine
{
	public:
		HttpRequest ( std::string const & str_request ) throw (ExceptionHttpStatusCode);
		HttpRequest ( HttpRequest const & model );
		HttpRequest & operator=(HttpRequest const & model);

		std::vector<std::string> const &getFieldValue(std::string const & filed_name, e_status &success_status) const;

	private:
		HttpRequest ( void );
		virtual ~HttpRequest( void );
		std::vector<std::string> _fields;
		
};
#endif