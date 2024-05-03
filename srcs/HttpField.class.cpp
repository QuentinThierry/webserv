/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpField.class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:52:09 by acardona          #+#    #+#             */
/*   Updated: 2024/05/03 22:19:54 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpField.class.hpp"

HttpField::HttpField ( void )
{}

HttpField::HttpField( std::string const & whole_line ) throw (ExceptionHttpStatusCode)
{
	size_t separator_position = whole_line.find(":");
	if (!str_contains_one_single_colon(whole_line, separator_position))
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_NO_UNIQUE_SEPARATOR);
	
	this->_setName(whole_line.substr(0, separator_position ));

	if (separator_position + 1 == std::string::npos)
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_NO_VALUE);
	else
		this->_addNewValues(whole_line.substr(separator_position + 1));

}

HttpField::HttpField(std::string const & name, std::string const & values) throw (ExceptionHttpStatusCode)
{
	_setName(name);
	_addNewValues(values);
}

HttpField::HttpField(HttpField const & model) : _name(model._name), _values(model._values)
{}

HttpField::~HttpField( void )
{}

HttpField & HttpField::operator=(HttpField const & model)
{
	if (&model == this)
	{
		_name = model._name;
		_values = model._values;
	}
	return ( *this);
}


std::string const & HttpField::getName( void ) const
{
	return (_name);
}

std::vector<std::string> const & HttpField::getValues( void ) const
{
	return (_values);
}


static bool	_is_field_authorized_whitespace( unsigned char const c )
{
	std::string whitespaces(HTTP_WHITESPACES);

	return ( whitespaces.find(c) != std::string::npos );
}

static bool _is_valid_values_char( unsigned char const c )
{
	return (is_VCHAR(c) || _is_field_authorized_whitespace(c));
}

//https://datatracker.ietf.org/doc/html/rfc7230#section-3.2
static	bool _is_values_list_syntax_ok( std::string const &str )
{
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		if (!_is_valid_values_char(str.at(i)))
			return (false);
	}
	return (true);
}

static	bool __is_one_value_syntax_ok( std::string const &str )
{
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		if (!is_VCHAR(str.at(i)))
			return (false);
	}
	return (true);
}

void	HttpField::_add_one_value( std::string const & str, size_t start_idx,
				size_t end_idx)
{
	std::string	value;

	value = str.substr(start_idx, end_idx - start_idx);
	trim_whitespace(value);
	if (! __is_one_value_syntax_ok(value))
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_INVALID_CHARACTER);
	if (value.size())
		_values.push_back(value);
}

static void	_skip_whitespaces(std::string & str, size_t & start_idx)
{
	size_t	str_size = str.size();
	while (start_idx < str_size
			&& (str.at(start_idx) == ','
				|| _is_field_authorized_whitespace(str.at(start_idx))))
	{
		++start_idx;
	}
}

static bool _is_valid_name_char( unsigned char const c )
{
	if (std::isalnum(c) || c == '_' || c == '-')
		return (true);
	return (false);
}

static bool _is_name_syntax_ok( std::string const &str )
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if ( ! _is_valid_name_char(*it) )
			return (false);
	}
	return (true);
}

void HttpField::_setName( std::string const &str )
{
	if (str.empty())
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_NO_NAME);
	if (! _is_name_syntax_ok(str) )
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_INVALID_CHARACTER);
	_name = str;
	format_string_to_canonical(_name);
	
}

void	HttpField::_add_all_values( std::string & str)
{
	size_t		start_idx, end_idx, str_size;

	str_size = str.size();
	start_idx = 0;
	while (start_idx < str_size)
	{
		_skip_whitespaces(str, start_idx);
		if (start_idx == str_size)
			return ;
		if (find_end_word(str, start_idx, ",", end_idx) == FAILURE)
			throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_QUOTE_NOT_CLOSED);
		_add_one_value(str, start_idx, end_idx);
		start_idx = end_idx;
	}
}

void	HttpField::_addNewValues( std::string str )
{
	if ( str.empty() )
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_EMPTY_ADDED_VALUE);
	if (_is_values_list_syntax_ok(str) == false)
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_INVALID_CHARACTER);
	_add_all_values(str);
	if (_values.size() == 0)
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_EMPTY_ADDED_VALUE);
}

void	HttpField::mergeFieldValues( HttpField &to_merge)
{
	const std::vector<std::string> values_to_add = to_merge.getValues();
	for (std::vector<std::string>::const_iterator it = values_to_add.begin() ; it != values_to_add.end(); ++it)
		_values.push_back(*it);
}

void	HttpField::display_field ( void ) const
{
	std::cout << _name << ": ";
	if (_values.size())
	{
		std::cout << _values[0];
		for (std::vector<std::string>::const_iterator it = ++(_values.begin());
			it != _values.end(); ++it)
		{
			std::cout << ", " << *it;
		}
	}
	std::cout << std::endl;
}


/*

//==============================================================

// General tests in the dedicated file.

// Individual functions tests:
// requieres to set all the HttpField member functions to public.


static void	_test__is_values_list_syntax_ok( void );
static void	_test___is_one_value_syntax_ok( void );
static void _test__add_one_value( void );
static void _test__add_all_values( void );
static void _test__addNewValues( void );
static void _test__setName( void );
static void _test_constructor( void );

int g_err_log_fd = STDERR_FILENO;

int main()
{
	_test__is_values_list_syntax_ok();
	_test___is_one_value_syntax_ok();
	_test__add_one_value();
	_test__add_all_values();
	_test__addNewValues();
	_test__setName();
	_test_constructor();
	return 0;

}

static void	_test__is_values_list_syntax_ok( void )
{
	std::vector<std::string>  fields;

	fields.push_back("");
	fields.push_back("thisiscorrect");
	fields.push_back("wrong space");
	fields.push_back("wrong\tspace");
	fields.push_back("wrong\nreturn");

	std::cout << "Test _is_values_list_syntax_ok : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
		std::cout << "- \""<< *it << "\" : " << _is_values_list_syntax_ok(*it) << std::endl;
	
	std::cout << std::endl;
}

static void	_test___is_one_value_syntax_ok( void )
{
	std::vector<std::string>  fields;

	fields.push_back("thisiscorrect");
	fields.push_back("thisis_correct");
	fields.push_back("thisis;correct");
	fields.push_back("thisis8correct");
	fields.push_back("");
	fields.push_back("wrong space");
	fields.push_back("wrong\tspace");

	std::cout << "Test __is_one_value_syntax_ok : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
		std::cout << "- \""<< *it << "\" : " << __is_one_value_syntax_ok(*it) << std::endl;

	std::cout << std::endl;
}

static void _test__add_one_value( void )
{
	HttpField new_field;

	std::vector<std::string>  fields;
	fields.push_back("simple_correct");
	fields.push_back("simple_correct, with_next");
	fields.push_back("\"quoted_correct\"abc");
	fields.push_back("\"quoted_correct\"continued");
	fields.push_back("\"quoted_correct\", next");
	fields.push_back("  simple_correct_space  ");
	fields.push_back("simple_correct;q=1");
	fields.push_back("incorrect space");

	std::cout << "Test _add_one_value : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\" (size = " << it->size() << "):" << std::endl;
		try
		{
			size_t	end_value;

			if (find_end_word(*it, 0, ",", end_value) == SUCCESS)
			{
				new_field._add_one_value(*it, 0, end_value);
				std::cout << " -> success: \"" << new_field.getValues().at(new_field.getValues().size() - 1) << "\"";
			}
			else
			{
				std::cout << " -> wrong quoting, test ignored" << std::endl; 
			}
		}
		catch (std::exception &e)
		{
			std::cout << " => not valid";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

static void _test__add_all_values( void )
{
	HttpField new_field;
	std::vector<std::string>  fields;
	fields.push_back("correct, first, \"second\", \"third-with-quotes\"123  ,,  ,  \"fourth,coma\", ,");
	fields.push_back("incorrect, forbidden space ");
	fields.push_back("incorrect, forbidden\ttab ");

	std::cout << "Test _add_all_values : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\":" << std::endl;
		try
		{
			new_field._add_all_values(*it);
			std::cout << " -> success: " << std::endl;
			for (std::vector<std::string>::const_iterator it = new_field.getValues().begin(); it != new_field.getValues().end(); ++it)
				std::cout << "   - \"" << *it << "\"" << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << " => not valid" << std::endl;
		}
	}
	std::cout << std::endl;
}

static void _test__addNewValues( void )
{
	HttpField new_field;
	std::vector<std::string>  fields;
	fields.push_back(",  , ");
	fields.push_back("");
	fields.push_back("correct, first, \"second\", \"third-with-quotes\"123  ,,  ,  \"fourth,coma\", ,");
	fields.push_back("incorrect, forbidden space ");
	fields.push_back("incorrect, forbidden\ttab ");
	fields.push_back("incorrect, forbidden\vchar ");

	std::cout << "Test _addNewValues : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\":" << std::endl;
		try
		{
			new_field._addNewValues(*it);
			std::cout << " -> success: " << std::endl;
			for (std::vector<std::string>::const_iterator it = new_field.getValues().begin(); it != new_field.getValues().end(); ++it)
				std::cout << "   - \"" << *it << "\"" << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << " => not valid" << std::endl;
		}
	}
	std::cout << std::endl;
}

static void _test__setName( void )
{
	HttpField new_field;
	std::vector<std::string>  fields;
	fields.push_back("correct");
	fields.push_back("cOrREct");
	fields.push_back("correct-special");
	fields.push_back("correct_special");
	fields.push_back("correct1digit");
	fields.push_back("incorrect.char");
	fields.push_back("incorrect char");
	fields.push_back("incorrectChar ");

	std::cout << "Test _setName : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\":" << std::endl;
		try
		{
			new_field._setName(*it);
			std::cout << " -> success: \"" << new_field.getName() << "\"" << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << " => not valid" << std::endl;
		}
	}
	std::cout << std::endl;
}

static void _test_constructor( void )
{
	std::vector<std::string>  fields;
	fields.push_back("correct_name:correct_value");
	fields.push_back("correct_name: correct_value_spaces ");
	fields.push_back("correct_name: correct_value_spaces , correct_value1");
	fields.push_back("incorrect name: correct_value");
	fields.push_back("name: incorrect value");
	fields.push_back("double_colon:correct_value:correct_value2");
	fields.push_back("no_colon");
	fields.push_back("no_colon correct_value");

	std::cout << "Test _constructor : " << std::endl;

	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		std::cout << "- \""<< *it << "\":" << std::endl;
		try
		{
			HttpField new_field(*it);
			std::cout << " -> success: " << std::endl;
			std::cout << "  - name: \"" << new_field.getName() << "\"" << std::endl;
			std::cout << "  - values: ";
			for (std::vector<std::string>::const_iterator it = new_field.getValues().begin(); it != new_field.getValues().end(); ++it)
			{
				std::cout << *it << ", ";
			}
			std::cout << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << " => not valid" << std::endl;
		}
	}
	std::cout << std::endl;
}

*/