#include "HttpRequestGet.class.hpp"

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

void			HttpRequestGet::process_header( Socket const * const socket )
{
	(void)socket;

	//TODO
}

bool	HttpRequestGet::hasBody() const
{
	return (false);
}

HttpResponse	HttpRequestGet::generate_response( Socket const * const socket )
{
	(void)socket;

	// Location location = socket->getServer().getLocation(string);// get location path
	// //get location cgi if cgi
	// //check method
	// if (checkMethod(location) == false)
	// {
	// 	throw ExceptionHttpStatusCode(HTTP_405);//!send error to client with allow method
	// }
	// if (location.getHasRedirect())
	//do redirection
	//check if uri exist
	//check if uri is a directory
		//check autoindex
		//check default file
	//return files
	return HttpResponse();
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket)
{
	(void)socket;
	(void)fd;


}
