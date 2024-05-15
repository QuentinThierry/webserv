#include "HttpRequestDelete.class.hpp"
#include "HttpResponse.class.hpp"

HttpRequestDelete::HttpRequestDelete (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest()
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPDELETE_SSTREAM_FAIL);
	HttpRequest::init(stream_request);
	if (*getMethod() != "DELETE")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPDELETE_WRONG_METHOD);
	//potential body unused then
}

HttpRequestDelete::HttpRequestDelete ( HttpRequestDelete const & model)
	: HttpRequest(model)
{
}

HttpRequestDelete & HttpRequestDelete::operator= (HttpRequestDelete const & model)
{
	if (&model != this)
		HttpRequest::operator=(model);
	return (*this);
}

HttpRequestDelete::HttpRequestDelete( void ) //unused
{
}

HttpRequestDelete::~HttpRequestDelete( void )
{
}

void			HttpRequestDelete::process_header( void )
{
	//TODO
}

HttpResponse	HttpRequestDelete::generate_response( void )
{
	//TODO
	return (HttpResponse());
}


bool	HttpRequestDelete::hasBody() const
{
	return (false);
}


#if 0

int g_err_log_fd = STDERR_FILENO;
std::vector<std::string> g_http_methods;
std::vector<std::string> g_http_versions;

int main ()
{
	_init_available_http_methods_versions();

	std::string request = "DELETE toto HTTP/1.1\r\nfield1:value1\r\n\r\nthis is the body\r\nit shouldn't appear";

	try
	{
		HttpRequestDelete get_request (request);

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