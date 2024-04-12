/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:32:04 by acardona          #+#    #+#             */
/*   Updated: 2024/04/10 21:42:49 by acardona         ###   ########.fr       */
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
		//TODO
		HttpRequestLine( void );
		std::vector<std::string> const &getFieldValue(std::string const & filed_name, e_status &success_status) const;
	private:
		//TODO

};

#endif