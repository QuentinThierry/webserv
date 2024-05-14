/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseStatus.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:47:33 by acardona          #+#    #+#             */
/*   Updated: 2024/05/14 19:21:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSESTATUS_HPP
# define HTTPRESPONSESTATUS_HPP

# include <sstream>

# include "Shared.hpp"

typedef enum
{
	HTTP_100,
	HTTP_101,
	HTTP_102,
	HTTP_103,
	HTTP_200,
	HTTP_201,
	HTTP_202,
	HTTP_203,
	HTTP_204,
	HTTP_205,
	HTTP_206,
	HTTP_207,
	HTTP_208,
	HTTP_226,
	HTTP_300,
	HTTP_301,
	HTTP_302,
	HTTP_303,
	HTTP_304,
	HTTP_305,
	HTTP_307,
	HTTP_308,
	HTTP_400,
	HTTP_401,
	HTTP_402,
	HTTP_403,
	HTTP_404,
	HTTP_405,
	HTTP_406,
	HTTP_407,
	HTTP_408,
	HTTP_409,
	HTTP_410,
	HTTP_411,
	HTTP_412,
	HTTP_413,
	HTTP_414,
	HTTP_415,
	HTTP_416,
	HTTP_417,
	HTTP_418,
	HTTP_421,
	HTTP_422,
	HTTP_423,
	HTTP_424,
	HTTP_425,
	HTTP_426,
	HTTP_428,
	HTTP_429,
	HTTP_431,
	HTTP_451,
	HTTP_500,
	HTTP_501,
	HTTP_502,
	HTTP_503,
	HTTP_504,
	HTTP_505,
	HTTP_506,
	HTTP_507,
	HTTP_508,
	HTTP_510,
	HTTP_511,
} e_status_code;

class ExceptionUnknownStatusCode : public std::exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("Wrong HTTP status code");
		}
};

std::string get_error_reason_phrase(int error_code) throw( ExceptionUnknownStatusCode);

class ExceptionHttpStatusCode : public std::exception 
{
	public:
		ExceptionHttpStatusCode( e_status_code status_code ) throw (ExceptionUnknownStatusCode) : _status_code (status_code) {}
		e_status_code const &	get_status_code( void ) const { return (_status_code);}
		void					display_error() const {std::cout << "Error " << get_status_code() << " " << get_error_reason_phrase(_status_code) << std::endl;}

	private:
		ExceptionHttpStatusCode( void ) {}
		e_status_code	_status_code;
};

#endif