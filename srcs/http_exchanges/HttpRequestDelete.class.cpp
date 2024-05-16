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

void			HttpRequestDelete::process_header( Socket const * const socket )
{
	(void)socket;
	//TODO
}

void	HttpRequestDelete::generate_response( Socket const * const socket, HttpResponse &response )
{
	//TODO
	(void)socket;
	(void)response;
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
