#include "HttpField.class.hpp"

HttpField::HttpField ( void )
{}

HttpField::HttpField( std::string const & whole_line ) throw (ExceptionHttpStatusCode)
{
	size_t separator_position = whole_line.find(":");
	if (!str_contains_a_colon(whole_line, separator_position))
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_NO_SEPARATOR);
	
	this->_setName(whole_line.substr(0, separator_position ));

	if (separator_position + 1 == std::string::npos)
		throw_http_err_with_log(HTTP_400, MSG_ERR_FIELD_NO_VALUE);
	else
		this->_addNewValues(whole_line.substr(separator_position + 1));

}

HttpField::HttpField(std::string const & name, std::vector<std::string> const & values )
{
	_name = name;
	_values = values;
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
	if (&model != this)
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
		
		if (is_whitespace(str.at(i)))
		{
			if (i != 0 && is_whitespace(str.at(i - 1)))
				return (false);
		}
		else if (!is_VCHAR(str.at(i)))
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

std::string	HttpField::getFields( void ) const
{
	std::string res;
	res = _name + ": ";
	if (_values.size())
	{
		res += _values[0];
		for (std::vector<std::string>::const_iterator it = ++(_values.begin());
			it != _values.end(); ++it)
		{
			res += ", " + *it;
		}
	}
	res += "\r\n";
	return res;
}

static std::vector<HttpField>::const_iterator _find_field(std::vector<HttpField> const &fields, std::string const & field_name)
{
	std::vector<HttpField>::const_iterator it;

	it = fields.begin();
	while ( it != fields.end())
	{
		if (it->getName() == field_name)
			return (it);
		++it;
	}
	return (it);
}

static std::vector<HttpField>::iterator _find_field(std::vector<HttpField> &fields, std::string const & field_name)
{
	std::vector<HttpField>::iterator it;

	it = fields.begin();
	while ( it != fields.end())
	{
		if (it->getName() == field_name)
			return (it);
		++it;
	}
	return (it);
}

static void _add_field(std::vector<HttpField> &existing_fields, HttpField new_field)
{
	std::vector<HttpField>::iterator it;

	it = _find_field(existing_fields, new_field.getName());
	if (it == existing_fields.end())
		existing_fields.push_back(new_field);
	else
		it->mergeFieldValues(new_field);
}


void	HttpField::fill_fields( std::stringstream &request_stream, std::vector<HttpField> &existing_fields)
	throw (ExceptionHttpStatusCode)
{
	std::string			line;

	while (!request_stream.eof() && std::getline(request_stream, line))
	{
		if (!is_line_properly_ended(request_stream, line))	
			throw_http_err_with_log(HTTP_400, MSG_ERR_WRONG_END_OF_LINE);
		if (line.empty())
			break;
		_add_field(existing_fields, HttpField(line));
	}
}


bool	HttpField::checkFieldExistence(std::vector<HttpField> const &fields, std::string const & field_name)
{
	return (_find_field(fields, field_name) != fields.end());
}

std::vector<std::string>	&HttpField::getFieldValue(
	std::vector<HttpField> &fields, std::string const & field_name)
	throw(ExceptionHttpStatusCode)
{
	std::vector<HttpField>::iterator it;

	it = _find_field(fields, field_name);
	if (it != fields.end())
		return (it->_values);
	throw(ExceptionHttpStatusCode(HTTP_500));
}

std::vector<std::string> const	&HttpField::getFieldValue(
	std::vector<HttpField> const &fields, std::string const & field_name)
	throw(ExceptionHttpStatusCode)
{
	std::vector<HttpField>::const_iterator it;

	it = _find_field(fields, field_name);
	if (it != fields.end())
		return (it->_values);
	throw(ExceptionHttpStatusCode(HTTP_500));
}

void	HttpField::erase_field(std::vector<HttpField> &fields,
	std::string const & field_name)
{
	std::vector<HttpField>::iterator it;

	it = _find_field(fields, field_name);
	if (it != fields.end())
		fields.erase(it);
}

void	HttpField::erase_field_value(HttpField &field, std::string const & value)
{
	std::vector<std::string>::iterator it;

	it = std::find(field._values.begin(), field._values.end(), value);
	if (it != field._values.end())
		field._values.erase(it);
}


e_status	HttpField::extract_field(std::vector<HttpField> &fields,
			std::string const & value, std::vector<std::string> &dest_values)
{
	std::vector<HttpField>::iterator it;
	std::vector<std::string> 		 values;

	it = _find_field(fields, value);
	if (it != fields.end())
	{
		dest_values = it->_values;
		fields.erase(it);
		return (SUCCESS);
	}
	return (FAILURE);
}