/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestLine.class.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 20:02:08 by acardona          #+#    #+#             */
/*   Updated: 2024/05/02 20:15:47 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequestLine.class.hpp"

HttpRequestLine::HttpRequestLine( void )
{
	//always call HttpRequestLine::_fill_request_line after calling this constructor
}

HttpRequestLine::HttpRequestLine( std::stringstream & request_stream) throw (ExceptionHttpStatusCode)
{
	std::string	first_line;

	getline(request_stream, first_line);
	_fill_request_line(first_line);
}

HttpRequestLine::HttpRequestLine( std::string const & request_first_line ) throw (ExceptionHttpStatusCode)
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
		throw (ExceptionHttpStatusCode(HTTP_400));
	return (first_word);
}

//Method :

static it_method	_match_token_to_method( std::string const & method_token )
{
	it_method	method;

	method = std::find(g_http_methods.begin(), g_http_methods.end(), method_token);
	if (method == g_http_methods.end())
	{
		throw (ExceptionHttpStatusCode(HTTP_501));
	}
	return (method);
}

static it_method	_check_and_get_method( std::stringstream & line )
{
	std::string	method_token;

	method_token = _extract_first_word( line);
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
		throw (ExceptionHttpStatusCode(HTTP_505));
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
		throw(ExceptionHttpStatusCode(HTTP_400));
}

void	HttpRequestLine::_fill_request_line( std::string const & request_first_line )
	throw (ExceptionHttpStatusCode)
{
	std::stringstream	line(request_first_line);

	_method = _check_and_get_method(line);
	_target = _check_and_get_target(line);
	_version = _check_and_get_version(line);
	_check_end_of_line(line);
}


/*
int main()
{
	try
	{
		HttpRequestLine line("POST /toto HTTP/0.9");
		std::cout << "Method: " << *line.getMethod() << std::endl;
		std::cout << "Target: " << line.getTarget() << std::endl;
		std::cout << "Version: " << *line.getVersion() << std::endl;
	} catch (ExceptionHttpStatusCode & e)
	{
		std::cout << "Error " << e.get_status_code() << " "
			<< get_error_reason_phrase(e.get_status_code()) << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << "Non http error: " << e.what() << std::endl;
	}
	return 0;
}
*/