/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 20:02:08 by acardona          #+#    #+#             */
/*   Updated: 2024/05/15 18:15:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequestLine.class.hpp"

HttpRequestLine::HttpRequestLine( void )
{
	//always call HttpRequestLine::_fill_request_line after calling this constructor
}

HttpRequestLine::HttpRequestLine( std::stringstream & request_stream)
	throw (ExceptionHttpStatusCode)
{
	_fill_request_line_with_stream(request_stream);
}

HttpRequestLine::HttpRequestLine( std::string const & request_first_line )
	throw (ExceptionHttpStatusCode)
{
	_fill_request_line(request_first_line);
}

HttpRequestLine::HttpRequestLine( HttpRequestLine const & model) :
	_method(model._method), _target(model._target), _version(model._version)
{
}

HttpRequestLine & HttpRequestLine::operator=(HttpRequestLine const & model )
{
	if (&model != this)
	{
		_method = model._method;
		_target = model._target;
		_version = model._version;
	}
	return (*this);
}


it_method const & HttpRequestLine::getMethod( void ) const
{
	return (_method);
}

std::string const & HttpRequestLine::getTarget( void ) const
{
	return (_target);
}

it_version const & HttpRequestLine::getVersion( void ) const
{
	return (_version);
}


static std::string _extract_first_word( std::stringstream &words )
{
	std::string	first_word;

	if (! (words >> first_word))
		throw_http_err_with_log(HTTP_400, MSG_ERR_LINE_MISSING_WORD);
	return (first_word);
}

//Method :
static it_method	_match_token_to_method( std::string const & method_token )
{
	it_method	method;

	method = std::find(g_http_methods.begin(), g_http_methods.end(), method_token);
	if (method == g_http_methods.end())
		throw_http_err_with_log(HTTP_501, MSG_ERR_LINE_WRONG_METHOD);
	return (method);
}
static it_method	_check_and_get_method( std::stringstream & line )
{
	std::string	method_token;

	method_token = _extract_first_word(line);
	return (_match_token_to_method(method_token));
}

//Target:
static std::string	_check_and_get_target( std::stringstream & line )
{
	return (_extract_first_word(line));
}

//Version:
static	it_version	_match_token_to_version( std::string const & version_token )
{
	it_method	version;

	version = std::find(g_http_versions.begin(), g_http_versions.end(), version_token);
	if (version == g_http_versions.end())
		throw_http_err_with_log(HTTP_505, MSG_ERR_LINE_WRONG_VERSION);
	return (version);
}

static it_version	_check_and_get_version( std::stringstream & line )
{
	std::string	version_token;

	version_token = _extract_first_word(line);
	return (_match_token_to_version(version_token));
}

static void	_check_end_of_line( std::stringstream & line )
{
	std::string	word;

	if (line >> word)
		throw_http_err_with_log(HTTP_400, MSG_ERR_LINE_TOO_MANY_WORDS);
}

void	HttpRequestLine::_fill_request_line( std::string const & request_first_line )
	throw (ExceptionHttpStatusCode)
{
	std::stringstream	line(request_first_line);
	if (line.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_LINE_SSTREAM_FAIL);
	_method = _check_and_get_method(line);
	_target = _check_and_get_target(line);
	_version = _check_and_get_version(line);
	_check_end_of_line(line);
}

void	HttpRequestLine::_fill_request_line_with_stream( std::stringstream & request_header_stream )
	throw (ExceptionHttpStatusCode)
{
	std::string	first_line;

	if (request_header_stream.eof()
		|| ! getline(request_header_stream, first_line))
		throw_http_err_with_log(HTTP_500, MSG_ERR_LINE_BAD_SSTREAM_INPUT);
	if (!is_line_properly_ended(request_header_stream, first_line))
		throw_http_err_with_log(HTTP_400, MSG_ERR_WRONG_END_OF_LINE);
	_fill_request_line(first_line);
}

void	HttpRequestLine::display_request_line( void ) const
{
	std::cout << *_method << " " << _target << " " << *_version << std::endl;
}
