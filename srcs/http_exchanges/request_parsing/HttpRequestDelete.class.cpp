#include "HttpRequestDelete.class.hpp"
#include "HttpResponse.class.hpp"
#include <sys/stat.h>
#include <fcntl.h>

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

void			HttpRequestDelete::process_header( Socket const * const socket )
{
	(void)socket;
	//TODO
}

bool	remove_directory(std::string & uri)
{
	int fd = open(uri.c_str(), O_DIRECTORY);
	if (fd == -1)
		return false;
	close(fd);
	//remove dir
	throw ExceptionHttpStatusCode(HTTP_403);
	return true;
}

void	remove_file(std::string & uri)
{
	if (remove(uri.c_str()) != 0)
	{
		if (errno == ENOENT)
			throw ExceptionHttpStatusCode(HTTP_404);
		else
			throw ExceptionHttpStatusCode(HTTP_403);
	}
}

void	HttpRequestDelete::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());
	response.setStatusCode(HTTP_204);

	Location location = socket->getServer().searchLocation(getTarget());// get location path
	if (response.handle_redirect(location))
		return ;
	if (checkMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		throw ExceptionHttpStatusCode(HTTP_405);
		return ;
	}
	std::string uri = getUri(location.getRootPath(), getTarget());
	response.addField("Content-Length", "0");
	// if (remove_directory(uri))
	// 	return FAILURE;
	remove_file(uri);
}

void	HttpRequestDelete::generate_response( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	response.fillHeader();
}

bool	HttpRequestDelete::hasBody() const
{
	return (false);
}

void	HttpRequestDelete::readBody(int fd, Socket const * const socket)
{
	(void)fd;
	(void)socket;
}
