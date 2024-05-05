#include "../includes/HttpRequestPost.class.hpp"
#include "../includes/HttpResponse.class.hpp"

static void	_add_body_from_request_stream( std::string &request_body,
				std::stringstream &stream_request )
{
	if (!stream_request.eof() && stream_request.peek() != EOF
			&& !std::getline(stream_request, request_body, '\0'))
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_SSTREAM_FAIL);
}

HttpRequestPost::HttpRequestPost (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest()
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_SSTREAM_FAIL);
	HttpRequest::init(stream_request);

	if (*getMethod() != "POST")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_WRONG_METHOD);

	_add_body_from_request_stream(_body, stream_request);



	//TODO you can complete the body with socket read read here or using addStringToBody outside of the constructor


}

HttpRequestPost::HttpRequestPost ( HttpRequestPost const & model)
	: HttpRequest(model)
{
		_body = model._body;
}

HttpRequestPost & HttpRequestPost::operator= (HttpRequestPost const & model)
{
	if (&model != this)
		HttpRequest::operator=(model);
	return (*this);
}

HttpRequestPost::HttpRequestPost( void ) //unused
{
}

HttpRequestPost::~HttpRequestPost( void )
{
}

std::string	const &	HttpRequestPost::getBody( void ) const
{
	return (_body);
}

void				HttpRequestPost::addStringToBody( std::string const & extra_body_content)
{
	_body += extra_body_content;
}

void	HttpRequestPost::display_request( void ) const
{
	HttpRequest::display_request();
	std::cout << std::endl <<  _body << std::endl;
}


void			HttpRequestPost::process_header( void )
{
	//TODO
}

HttpResponse	HttpRequestPost::generate_response( void )
{
	//TODO
	return (HttpResponse());
}


#if 0

int g_err_log_fd = STDERR_FILENO;

int main ()
{
	std::string request = "POST the_target/ HTTP/1.1\nfield1:value1\nField2:value1\n\nThis body is amazing\nDon't you think so?";

	try
	{
		HttpRequestPost get_request (request);

		get_request.display_request();
	}
	catch (ExceptionHttpStatusCode & e)
	{
		e.display_error();
	}
	catch (std::exception & e)
	{
		std::cout << "Non http error: " << e.what() << std::endl;
	}
	return (true);
}

#endif