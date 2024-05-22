#include "HttpRequestGet.class.hpp"
#include <sys/stat.h>

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
}

bool	HttpRequestGet::hasBody() const
{
	return (false);
}

bool	handle_directory(std::string & uri, Location const & location, HttpResponse & response)
{
	if (location.getHasAutoindex())
	{
		//add content-length flags
		//fill body auto index
		response.fillHeader();
		return true; //TODO
	}
	if (location.updateUriToIndex(uri) == SUCCESS)
		return false;
	throw ExceptionHttpStatusCode(HTTP_403);
}

void	handle_file(std::string & uri, HttpResponse & response)
{
	e_status_code error_code = response.openFstream(uri);
	if (error_code != HTTP_200)
	{
		throw ExceptionHttpStatusCode(error_code);
	}
}

void	HttpRequestGet::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());

	Location location = socket->getServer().searchLocation(getTarget());// get location path
	//get location cgi if cgi
	if (response.handle_redirect(location))
		return ;
	if (isAcceptedMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		throw ExceptionHttpStatusCode(HTTP_405);
		return ;
	}
	std::string uri = getUri(location.getRootPath(), getTarget());
	if (is_accessible_directory(uri.c_str()))
		if (handle_directory(uri, location, response))
			return ;
	handle_file(uri, response);
}

void	HttpRequestGet::generate_response( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	response.fillHeader();
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket)
{
	(void)socket;
	(void)fd;
}
