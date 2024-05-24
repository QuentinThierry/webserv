/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseStatus.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:55:25 by acardona          #+#    #+#             */
/*   Updated: 2024/05/24 14:34:49 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseStatus.hpp"


std::string get_error_reason_phrase(int error_code) throw( ExceptionUnknownStatusCode)
{
	switch (error_code)
	{
		case HTTP_100:
			return ("Continue");
		case HTTP_101:
			return ("Switching Protocols");
		case HTTP_102:
			return ("Processing");
		case HTTP_103:
			return ("Early Hints");
		case HTTP_200:
			return ("OK");
		case HTTP_201:
			return ("Created");
		case HTTP_202:
			return ("Accepted");
		case HTTP_203:
			return ("Non-Authoritative Information");
		case HTTP_204:
			return ("No Content");
		case HTTP_205:
			return ("Reset Content");
		case HTTP_206:
			return ("Partial Content");
		case HTTP_207:
			return ("Multi-Status");
		case HTTP_208:
			return ("Already Reported");
		case HTTP_226:
			return ("IM Used");
		case HTTP_300:
			return ("Multiple Choices");
		case HTTP_301:
			return ("Moved Permanently");
		case HTTP_302:
			return ("Found");
		case HTTP_303:
			return ("See Other");
		case HTTP_304:
			return ("Not Modified");
		case HTTP_305:
			return ("Use Proxy ");
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

e_status_code	int_to_status_code(uint16_t number)
{
	switch (number)
	{
		case 100:
			return HTTP_100;
		case 101:
			return HTTP_101;
		case 102:
			return HTTP_102;
		case 103:
			return HTTP_103;
		case 200:
			return HTTP_200;
		case 201:
			return HTTP_201;
		case 202:
			return HTTP_202;
		case 203:
			return HTTP_203;
		case 204:
			return HTTP_204;
		case 205:
			return HTTP_205;
		case 206:
			return HTTP_206;
		case 207:
			return HTTP_207;
		case 208:
			return HTTP_208;
		case 226:
			return HTTP_226;
		case 300:
			return HTTP_300;
		case 301:
			return HTTP_301;
		case 302:
			return HTTP_302;
		case 303:
			return HTTP_303;
		case 304:
			return HTTP_304;
		case 305:
			return HTTP_305;
		case 307:
			return HTTP_307;
		case 308:
			return HTTP_308;
		case 400:
			return HTTP_400;
		case 401:
			return HTTP_401;
		case 402:
			return HTTP_402;
		case 403:
			return HTTP_403;
		case 404:
			return HTTP_404;
		case 405:
			return HTTP_405;
		case 406:
			return HTTP_406;
		case 407:
			return HTTP_407;
		case 408:
			return HTTP_408;
		case 409:
			return HTTP_409;
		case 410:
			return HTTP_410;
		case 411:
			return HTTP_411;
		case 412:
			return HTTP_412;
		case 413:
			return HTTP_413;
		case 414:
			return HTTP_414;
		case 415:
			return HTTP_415;
		case 416:
			return HTTP_416;
		case 417:
			return HTTP_417;
		case 418:
			return HTTP_418;
		case 421:
			return HTTP_421;
		case 422:
			return HTTP_422;
		case 423:
			return HTTP_423;
		case 424:
			return HTTP_424;
		case 425:
			return HTTP_425;
		case 426:
			return HTTP_426;
		case 428:
			return HTTP_428;
		case 429:
			return HTTP_429;
		case 431:
			return HTTP_431;
		case 451:
			return HTTP_451;
		case 500:
			return HTTP_500;
		case 501:
			return HTTP_501;
		case 502:
			return HTTP_502;
		case 503:
			return HTTP_503;
		case 504:
			return HTTP_504;
		case 505:
			return HTTP_505;
		case 506:
			return HTTP_506;
		case 507:
			return HTTP_507;
		case 508:
			return HTTP_508;
		case 510:
			return HTTP_510;
		case 511:
			return HTTP_511;
		default:
			throw (ExceptionUnknownStatusCode());
	}
}

uint32_t	status_code_to_int(e_status_code status_code)
{
	switch (status_code)
	{
		case HTTP_100:
			return 100;
		case HTTP_101:
			return 101;
		case HTTP_102:
			return 102;
		case HTTP_103:
			return 103;
		case HTTP_200:
			return 200;
		case HTTP_201:
			return 201;
		case HTTP_202:
			return 202;
		case HTTP_203:
			return 203;
		case HTTP_204:
			return 204;
		case HTTP_205:
			return 205;
		case HTTP_206:
			return 206;
		case HTTP_207:
			return 207;
		case HTTP_208:
			return 208;
		case HTTP_226:
			return 226;
		case HTTP_300:
			return 300;
		case HTTP_301:
			return 301;
		case HTTP_302:
			return 302;
		case HTTP_303:
			return 303;
		case HTTP_304:
			return 304;
		case HTTP_305:
			return 305;
		case HTTP_307:
			return 307;
		case HTTP_308:
			return 308;
		case HTTP_400:
			return 400;
		case HTTP_401:
			return 401;
		case HTTP_402:
			return 402;
		case HTTP_403:
			return 403;
		case HTTP_404:
			return 404;
		case HTTP_405:
			return 405;
		case HTTP_406:
			return 406;
		case HTTP_407:
			return 407;
		case HTTP_408:
			return 408;
		case HTTP_409:
			return 409;
		case HTTP_410:
			return 410;
		case HTTP_411:
			return 411;
		case HTTP_412:
			return 412;
		case HTTP_413:
			return 413;
		case HTTP_414:
			return 414;
		case HTTP_415:
			return 415;
		case HTTP_416:
			return 416;
		case HTTP_417:
			return 417;
		case HTTP_418:
			return 418;
		case HTTP_421:
			return 421;
		case HTTP_422:
			return 422;
		case HTTP_423:
			return 423;
		case HTTP_424:
			return 424;
		case HTTP_425:
			return 425;
		case HTTP_426:
			return 426;
		case HTTP_428:
			return 428;
		case HTTP_429:
			return 429;
		case HTTP_431:
			return 431;
		case HTTP_451:
			return 451;
		case HTTP_500:
			return 500;
		case HTTP_501:
			return 501;
		case HTTP_502:
			return 502;
		case HTTP_503:
			return 503;
		case HTTP_504:
			return 504;
		case HTTP_505:
			return 505;
		case HTTP_506:
			return 506;
		case HTTP_507:
			return 507;
		case HTTP_508:
			return 508;
		case HTTP_510:
			return 510;
		case HTTP_511:
			return 511;
		default:
			throw (ExceptionUnknownStatusCode());
	}
}