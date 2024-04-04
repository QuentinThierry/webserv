/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseStatus.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:55:25 by acardona          #+#    #+#             */
/*   Updated: 2024/04/04 19:58:12 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpResponseStatus.hpp"


std::string get_error_reason_phrase(int error_code) throw( ExceptionUnknownStatusCode)
{
	switch (error_code)
	{
		case HTTP_307:
			return ("Temporary Redirect");
		case HTTP_308:
			return ("Permanent Redirect");
		case HTTP_400:
			return ("Bad Request");
		case HTTP_401:
			return ("Unauthorized");
		case HTTP_402:
			return ("Payment Required");
		case HTTP_403:
			return ("Forbidden");
		case HTTP_404:
			return ("Not Found");
		case HTTP_405:
			return ("Method Not Allowed");
		case HTTP_406:
			return ("Not Acceptable");
		case HTTP_407:
			return ("Proxy Authentication Required");
		case HTTP_408:
			return ("Request Timeout");
		case HTTP_409:
			return ("Conflict");
		case HTTP_410:
			return ("Gone");
		case HTTP_411:
			return ("Length Required");
		case HTTP_412:
			return ("Precondition Failed");
		case HTTP_413:
			return ("Payload Too Large");
		case HTTP_414:
			return ("URI Too Long");
		case HTTP_415:
			return ("Unsupported Media Type");
		case HTTP_416:
			return ("Range Not Satisfiable");
		case HTTP_417:
			return ("Expectation Failed");
		case HTTP_418:
			return ("I'm a Teapot");
		case HTTP_421:
			return ("Misdirected Request");
		case HTTP_422:
			return ("Unprocessable Entity");
		case HTTP_423:
			return ("Locked");
		case HTTP_424:
			return ("Failed Dependency");
		case HTTP_425:
			return ("Too Early");
		case HTTP_426:
			return ("Upgrade Required");
		case HTTP_428:
			return ("Precondition Required");
		case HTTP_429:
			return ("Too Many Requests");
		case HTTP_431:
			return ("Request Header Fields Too Large");
		case HTTP_451:
			return ("Unavailable For Legal Reasons");
		case HTTP_500:
			return ("Internal Server Error");
		case HTTP_501:
			return ("Not Implemented");
		case HTTP_502:
			return ("Bad Gateway");
		case HTTP_503:
			return ("Service Unavailable");
		case HTTP_504:
			return ("Gateway Timeout");
		case HTTP_505:
			return ("HTTP Version Not Supported");
		case HTTP_506:
			return ("Variant Also Negotiates");
		case HTTP_507:
			return ("Insufficient Storage");
		case HTTP_508:
			return ("Loop Detected");
		case HTTP_510:
			return ("Not Extended");
		case HTTP_511:
			return ("Network Authentication Required");
		default:
			throw (ExceptionUnknownStatusCode());
	}
}