#include "../includes/HttpRequestGet.class.hpp"
#include "../includes/HttpResponse.class.hpp"

HttpRequestGet::HttpRequestGet (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest()
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPGET_SSTREAM_FAIL);
	HttpRequest::init(stream_request);
	if (*getMethod() != "GET")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPGET_WRONG_METHOD);
	//potential body unused then
}

HttpRequestGet::HttpRequestGet ( HttpRequestGet const & model)
	: HttpRequest(model)
{
}

HttpRequestGet & HttpRequestGet::operator= (HttpRequestGet const & model)
{
	if (&model != this)
		HttpRequest::operator=(model);
	return (*this);
}

HttpRequestGet::HttpRequestGet( void ) //unused
{
}

HttpRequestGet::~HttpRequestGet( void )
{
}

void			HttpRequestGet::process_header( void )
{
	//TODO
}

HttpResponse	HttpRequestGet::generate_response( void )
{
	//TODO
	return (HttpResponse());
}

bool	HttpRequestGet::hasBody() const
{
	return (false);
}



#if 0

int g_err_log_fd = STDERR_FILENO;

int main ()
{
	_init_available_http_methods_versions();

	std::string request = "GET toto HTTP/1.1\nfield1:value1\n\nthis is the body\nit shouldn't appear";

	try
	{
		HttpRequestGet get_request (request);

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