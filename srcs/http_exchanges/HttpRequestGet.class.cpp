#include "HttpRequestGet.class.hpp"
#include "HttpResponse.class.hpp"

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
std::vector<std::string> g_http_methods;
std::vector<std::string> g_http_versions;

int main ()
{
	_init_available_http_methods_versions();

	std::string request = "GET / HTTP/1.1\r\nHost: localhost:8081\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nSec-Fetch-Dest: document\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-Site: none\r\nSec-Fetch-User: ?1";

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