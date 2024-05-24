/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpField.class.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:52:09 by acardona          #+#    #+#             */
/*   Updated: 2024/05/14 19:11:04 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPFIELD_CLASS_HPP
# define HTTPFIELD_CLASS_HPP

# include "HttpTools.hpp"

# include <vector>

# define MSG_ERR_FIELD_NO_SEPARATOR			"Error: HTTP request: a field does not contain a ':' separator"
# define MSG_ERR_FIELD_NO_VALUE				"Error: HTTP request: a field does not contain any value"
# define MSG_ERR_FIELD_NO_NAME				"Error: HTTP request: a field does not contain any name"
# define MSG_ERR_FIELD_QUOTE_NOT_CLOSED		"Error: HTTP request: quote note closed in a field"
# define MSG_ERR_FIELD_INVALID_CHARACTER	"Error: HTTP request: invalid character in a field"
# define MSG_ERR_FIELD_EMPTY_ADDED_VALUE	"Error: HTTP request: attempted to add an empty value to a field"


class HttpField
{
	public:
		HttpField( std::string const & whole_line ) throw (ExceptionHttpStatusCode);
		HttpField( std::string const & name, std::string const & values ) throw (ExceptionHttpStatusCode);
		HttpField( std::string const & name, std::vector<std::string> const & values ) throw (ExceptionHttpStatusCode);
		HttpField( HttpField const & model );
		HttpField & operator=( HttpField const & model );
		virtual ~HttpField( void );

		std::string const &					getName( void ) const;
		std::vector<std::string> const &	getValues( void ) const;
		std::string							getFields( void ) const;
		void								mergeFieldValues( HttpField &to_merge);

		void	display_field ( void ) const;

	private:
		std::string					_name;
		std::vector<std::string>	_values;
		
		HttpField( void );
		void	_addNewValues( std::string str );
		void	_add_one_value( std::string const & str, size_t start_idx,
					size_t end_idx);
		void	_add_all_values( std::string & str);
		void	_setName( std::string const &str );

};

#endif