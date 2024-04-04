/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpField.class.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:52:09 by acardona          #+#    #+#             */
/*   Updated: 2024/04/04 19:58:26 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPFIELD_CLASS_HPP
# define HTTPFIELD_CLASS_HPP

# include "shared.hpp"
# include "HttpResponseStatus.hpp"


class HttpField
{
	public:
		HttpField( std::string const & whole_line ) throw (ExceptionHttpStatusCode);
		HttpField( std::string const & name, std::string const & values );
		HttpField( HttpField const & model );
		virtual ~HttpField( void );
		HttpField & operator=( HttpField const & model );

		std::string const & getName( void ) const;
		std::string const & getValues( void ) const;
		
	private:
		HttpField( void );
		std::string	_name;
		std::string	_values;
};

#endif