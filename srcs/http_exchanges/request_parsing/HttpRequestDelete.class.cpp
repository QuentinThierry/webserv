	#include "HttpRequestDelete.class.hpp"
#include "HttpRequestPost.class.hpp"
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

HttpRequestDelete::HttpRequestDelete( void )
{
}

HttpRequestDelete & HttpRequestDelete::operator= (HttpRequestDelete const & model)
{
	if (&model != this)
		HttpRequest::operator=(model);
	return (*this);
}

HttpRequestDelete::~HttpRequestDelete( void )
{
}

bool	HttpRequestDelete::hasCgi() const {return false;}

void	HttpRequestDelete::setCgi(bool has_cgi) {(void)has_cgi;}

Cgi		*HttpRequestDelete::getCgi() {return NULL;}

void	HttpRequestDelete::processHeader( Socket const * const socket ) {(void)socket;}

bool	HttpRequestDelete::hasBody() const {return (false);}

void	HttpRequestDelete::readBody(int fd, Socket const * const socket, bool &end)
{
	(void)fd;
	(void)socket;
	end = true;
}

static void	remove_file(std::string const & uri)
{
	if (HttpRequestPost::isBusyFile(uri))
		throw_http_err_with_log(HTTP_404, "ERROR: no such file or directory");
	if (remove(uri.c_str()) != 0)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			throw_http_err_with_log(HTTP_404, "ERROR: no such file or directory");
		else
			throw_http_err_with_log(HTTP_403, "ERROR: no such file or directory");
	}
}

void	HttpRequestDelete::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());
	response.setStatusCode(HTTP_204);

	Location location = socket->getServer().searchLocation(getTarget());
	if (response.handle_redirect(location))
		return ;
	if (isAcceptedMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		throw_http_err_with_log(HTTP_405, "ERROR: method not allowed");
		return ;
	}
	std::string uri = getUri(location.getRootPath());
	response.addField("Content-Length", "0");
	remove_file(uri);
}

void	HttpRequestDelete::generateResponse( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	response.fillHeader();
}
