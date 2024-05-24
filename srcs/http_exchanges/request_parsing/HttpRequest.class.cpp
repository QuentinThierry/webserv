#include "HttpRequest.class.hpp"

bool HttpRequest::isAcceptedMethod(Location const & location) const
{
	return (location.doesAcceptMethod(*getMethod()));
}

HttpRequest::HttpRequest ( void ) : HttpRequestLine()
{
	// a call to HttpRequest::init must be done after calling this constructor
}

HttpRequest::HttpRequest ( HttpRequest const & model ) :
	HttpRequestLine(model), _body(model._body), _fields(model._fields)
{
}

HttpRequest & HttpRequest::operator=(HttpRequest const & model)
{
	if (this != &model)
	{
		HttpRequestLine::operator=(model);
		_fields = model._fields;
		_body = model._body;
	}
	return (*this);
}

void	HttpRequest::init( std::stringstream &request_stream ) throw(ExceptionHttpStatusCode)
{
	_fill_request_line_with_stream(request_stream);
	_fill_fields(request_stream);
}

bool	HttpRequest::checkFieldExistence(std::string const & field_name) const
{
	std::vector<HttpField>::const_iterator it;

	for (it = _fields.begin(); it != _fields.end(); ++it)
	{
		if (it->getName() == field_name)
			return (true);
	}
	return (false);
}

const std::vector<std::string>	&HttpRequest::getFieldValue(
	std::string const & field_name) const throw(ExceptionHttpStatusCode)
{
	std::vector<HttpField>::const_iterator it;

	for (it = _fields.begin(); it != _fields.end(); ++it)
	{
		if (it->getName() == field_name)
			return (it->getValues());
	}
	throw(ExceptionHttpStatusCode(HTTP_500));
}

HttpRequest::~HttpRequest( void )
{
}

static std::vector<HttpField>::iterator	_get_field(std::vector<HttpField> &fields, std::string const & field_name)
{
	std::vector<HttpField>::iterator it;

	for (it = fields.begin(); it != fields.end(); ++it)
	{
		if (it->getName() == field_name)
			return (it);
	}
	return (it);
}

static void _add_field(std::vector<HttpField> &existing_fields, HttpField new_field)
{
	std::vector<HttpField>::iterator it;

	it = _get_field(existing_fields, new_field.getName());
	if (it == existing_fields.end())
		existing_fields.push_back(new_field);
	else
		it->mergeFieldValues(new_field);
}

void	HttpRequest::_fill_fields( std::stringstream &request_stream)
	throw (ExceptionHttpStatusCode)
{
	std::string			line;

	while (!request_stream.eof() && std::getline(request_stream, line))
	{
		if (!is_line_properly_ended(request_stream, line))	
			throw_http_err_with_log(HTTP_400, MSG_ERR_WRONG_END_OF_LINE);
		if (line.empty())
			break;
		_add_field(_fields, HttpField(line));
	}
}

void	HttpRequest::displayRequest( void ) const
{
	displayRequest_line();
	for (std::vector<HttpField>::const_iterator it = _fields.begin();
			it != _fields.end(); ++it)
	{
		it->display_field();
	}
	if (!_body.empty())
		std::cout << std::endl << _body << std::endl;
}


std::string const &	HttpRequest::getBody( void ) const
{
	return (_body);
}

void				HttpRequest::addStringToBody( std::string const & extra_body_content)
{
	_body += extra_body_content;
}
