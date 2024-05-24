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

void			HttpRequestDelete::processHeader( Socket const * const socket )
{
	(void)socket;
	//TODO
}

bool	remove_directory(std::string & uri, HttpResponse & response, Server const & server)
{
	int fd = open(uri.c_str(), O_DIRECTORY);
	if (fd == -1)
		return false;
	close(fd);
	//remove dir
	response.generateErrorResponse(HTTP_403, server);
	return true;
}

e_status	remove_file(std::string & uri, HttpResponse & response, Server const & server)
{
	if (remove(uri.c_str()) != 0)
	{
		if (errno == ENOENT)
			response.generateErrorResponse(HTTP_404, server);
		else
			response.generateErrorResponse(HTTP_403, server);
		return FAILURE;
	}
	return SUCCESS;
}

e_status	HttpRequestDelete::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());
	response.setStatusCode(HTTP_204);

	Location location = socket->getServer().searchLocation(getTarget());// get location path
	if (response.handle_redirect(location))
		return SUCCESS;
	if (isAcceptedMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		response.generateErrorResponse(HTTP_405, socket->getServer());
		return FAILURE;
	}
	std::string uri = getUri(location.getRootPath(), getTarget());
	response.addField("Content-Length", "0");
	// if (remove_directory(uri, response, socket->getServer()))
	// 	return FAILURE;
	return remove_file(uri, response, socket->getServer());
}

void	HttpRequestDelete::generateResponse( Socket const * const socket, HttpResponse &response )
{
	if (_initResponse(socket, response) == SUCCESS)
		response.fillHeader();
	return ;
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
