/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseStatus.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:47:33 by acardona          #+#    #+#             */
/*   Updated: 2024/05/05 18:57:15 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSESTATUS_HPP
# define HTTPRESPONSESTATUS_HPP

# include <sstream>

# include "Shared.hpp"

typedef enum 
{
	HTTP_100 = 100,
	HTTP_101 = 101,
	HTTP_102 = 102,
	HTTP_103 = 103,
	HTTP_200 = 200,
	HTTP_201 = 201,
	HTTP_202 = 202,
	HTTP_203 = 203,
	HTTP_204 = 204,
	HTTP_205 = 205,
	HTTP_206 = 206,
	HTTP_207 = 207,
	HTTP_208 = 208,
	HTTP_226 = 226,
	HTTP_300 = 300,
	HTTP_301 = 301,
	HTTP_302 = 302,
	HTTP_303 = 303,
	HTTP_304 = 304,
	HTTP_305 = 305,
	HTTP_306 = 306,
	HTTP_307 = 307,
	HTTP_308 = 308,
	HTTP_400 = 400,
	HTTP_401 = 401,
	HTTP_402 = 402,
	HTTP_403 = 403,
	HTTP_404 = 404,
	HTTP_405 = 405,
	HTTP_406 = 406,
	HTTP_407 = 407,
	HTTP_408 = 408,
	HTTP_409 = 409,
	HTTP_410 = 410,
	HTTP_411 = 411,
	HTTP_412 = 412,
	HTTP_413 = 413,
	HTTP_414 = 414,
	HTTP_415 = 415,
	HTTP_416 = 416,
	HTTP_417 = 417,
	HTTP_418 = 418,
	HTTP_421 = 421,
	HTTP_422 = 422,
	HTTP_423 = 423,
	HTTP_424 = 424,
	HTTP_425 = 425,
	HTTP_426 = 426,
	HTTP_428 = 428,
	HTTP_429 = 429,
	HTTP_431 = 431,
	HTTP_451 = 451,
	HTTP_500 = 500,
	HTTP_501 = 501,
	HTTP_502 = 502,
	HTTP_503 = 503,
	HTTP_504 = 504,
	HTTP_505 = 505,
	HTTP_506 = 506,
	HTTP_507 = 507,
	HTTP_508 = 508,
	HTTP_510 = 510,
	HTTP_511 = 511,
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