#include "../includes/HttpRequest.class.hpp"


HttpRequest::HttpRequest ( void ) : HttpRequestLine()
{
	// a call to HttpRequest::init must be done after calling this constructor
}

HttpRequest::HttpRequest ( HttpRequest const & model ) : HttpRequestLine(model)
{
	_fields = model._fields;
}

HttpRequest & HttpRequest::operator=(HttpRequest const & model)
{
	if (this != &model)
	{
		HttpRequestLine::operator=(model);
		_fields = model._fields;
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

	while (!request_stream.eof() && std::getline(request_stream, line)
		&& !line.empty())
	{
		_add_field(_fields, HttpField(line));
	}
}

void	HttpRequest::display_request( void ) const
{
	display_request_line();
	for (std::vector<HttpField>::const_iterator it = _fields.begin();
			it != _fields.end(); ++it)
	{
		it->display_field();
	}
}



/*
int g_err_log_fd = STDERR_FILENO;

int main()
{
	std::string	request_str = "GET ./toto HTTP/0.9\nfield1:value1\nfield2:value2\nfield1: value3";
	std::stringstream request_stream(request_str);

	HttpRequest	request;
	try
	{
		request.init(request_stream);
		std::cout << "Success:" << std::endl;
		request.display_request();
	}
	catch (ExceptionHttpStatusCode & e)
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