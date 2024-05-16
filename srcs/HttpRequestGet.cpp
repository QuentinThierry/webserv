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

std::string	getUri(std::string root, std::string target)
{
	return (root + target);
}

bool	handle_directory(std::string & uri, Location const & location, HttpResponse & response, Server const & server)
{
	//check if uri is a directory
	int fd = open(uri.c_str(), O_DIRECTORY);
	if (fd == -1)
		return false;
	close(fd);
	if (location.getHasAutoindex())
	{
		//add content-lenght flags
		//fill body auto index
		response.fillHeader();
		return true; //TODO
	}
	std::vector<std::string> index = location.getDefaultDirPath();
	for (unsigned int i = 0; i < index.size(); i++)
	{
		// std::cout << index.at(i) << std::endl;
		if (access((uri + index.at(i)).c_str(), F_OK) != -1)
		{
			// std::cout << "match\n";
			std::cout << uri <<std::endl;
			std::cout << uri + index.at(i) << std::endl;
			uri = uri + index.at(i);
			return false;
		}
	}
	response.generateErrorResponse(HTTP_403, server);
	return true;
}

e_status	handle_file(std::string & uri, HttpResponse & response, Server const & server)
{
	e_status_code error_code = response.openFstream(uri);
	if (error_code != HTTP_200)
	{
		response.generateErrorResponse(error_code, server);
		return FAILURE;
	}
	return SUCCESS;
}

e_status	HttpRequestGet::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());

	Location location = socket->getServer().searchLocation(getTarget());// get location path
	//get location cgi if cgi
	if (response.handle_redirect(location))
		return SUCCESS;
	if (checkMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		response.generateErrorResponse(HTTP_405, socket->getServer());
		return FAILURE;
	}
	std::string uri = getUri(location.getRootPath(), getTarget());
	if (handle_directory(uri, location, response, socket->getServer()))
		return FAILURE;
	return handle_file(uri, response, socket->getServer());
}

void	HttpRequestGet::generate_response( Socket const * const socket, HttpResponse &response )
{
	if (_initResponse(socket, response) == SUCCESS)
		response.fillHeader();
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket)
{
	(void)socket;
	(void)fd;
}
