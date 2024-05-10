#include "HttpRequestGet.class.hpp"
#include <sys/stat.h>
#include <fcntl.h>

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

std::string	getUri(std::string root, std::string target)
{
	return (root + target);
}

bool	checktUriExist(std::string uri, int permission)
{
	return (access(uri.c_str(), permission) == 0 ? true : false);
}

bool	handle_directory(std::string & uri, Location const & location, HttpResponse & response)
{
	//check if uri is a directory
	int fd = open(uri.c_str(), O_DIRECTORY);
	if (fd == -1)
		return false;
	close(fd);
	//check autoindex
	if (location.getHasAutoindex())
		return true; //TODO
	//check default file => index
	std::vector<std::string> index = location.getDefaultDirPath();
	for (unsigned int i = 0; i < index.size(); i++)
	{
		if (access((uri + index.at(i)).c_str(), F_OK) != -1)
		{
			uri = uri + index.at(i);
			response.setStatusCode(HTTP_304);
			return false;
		}
	}
	response.setStatusCode(HTTP_403);
	return true;
}

HttpResponse	HttpRequestGet::generate_response( Socket const * const socket )
{
	HttpResponse response(getVersion());

	Location location = socket->getServer().getLocation(getTarget());// get location path
	//get location cgi if cgi
	if (checkMethod(location) == false)
	{
		response.generateErrorResponse(HTTP_405);//!send error to client with allow method
		return response;
	}
	std::string uri = getUri(location.getRootPath(), getTarget());
	if (!checktUriExist(uri, R_OK))
	{
		response.generateErrorResponse(HTTP_403);
		return response;
	}
	if (response.handle_redirect(location))
		return response;
	if (handle_directory(uri, location, response))
		return response;
	
	//return files
	return response;
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket)
{
	(void)socket;
	(void)fd;


}
