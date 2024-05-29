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

HttpRequestGet::HttpRequestGet( void )
{
}

HttpRequestGet::~HttpRequestGet( void )
{
}

void			HttpRequestGet::processHeader( Socket const * const socket )
{
	(void)socket;
}

bool	HttpRequestGet::hasBody() const
{
	return (false);
}


void HttpRequestGet::_handleCgi(std::string & uri, Server const & server, CgiLocation const &cgi_location)
{
	_cgi.exec(cgi_location.getExecPath(), uri, *this, server);
}

void	HttpRequestGet::_handle_file(std::string & uri, HttpResponse & response, Server const & server)
{
	CgiLocation cgi_location;
	_has_cgi = server.searchCgiLocation(uri, cgi_location);
	if (_has_cgi)
		return _handleCgi(uri, server, cgi_location);
	e_status_code error_code = response.openBodyFileStream(uri);
	if (error_code != HTTP_200)
		throw ExceptionHttpStatusCode(error_code);
}

static void	_handle_Autoindex(std::string & uri, Location const & location)
{
		//add content-length flags
		//fill body auto index
		(void) uri;
		(void) location;
		return; //TODO	
}

e_status	HttpRequestGet::_handle_index_file(std::string & uri, Location const & location,
				HttpResponse & response,  Server const & server)
{
	if (location.updateUriToIndex(uri) == SUCCESS)
	{
		_handle_file(uri, response, server);
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

void HttpRequestGet::_redirectDirectory(HttpResponse & response)
{
	std::string location = "http://";
	location += getFieldValue("Host")[0];
	location += getTarget();
	location += "/";
	response.addField("Location", location);
	throw ExceptionHttpStatusCode(HTTP_301);
}

void	HttpRequestGet::_handleDirectory(std::string & uri, Location const & location,
				HttpResponse & response, Server const & server)
{
	if (*(uri.end() - 1) != '/')
		_redirectDirectory(response);
	if (_handle_index_file(uri, location, response, server) == SUCCESS)
		return ;
	if (location.getHasAutoindex())
	{
		_handle_Autoindex(uri, location);
		return ;
	}
	throw ExceptionHttpStatusCode(HTTP_403);
}

void	HttpRequestGet::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());

	Location location = socket->getServer().searchLocation(getTarget());
	
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
		_handleDirectory(uri, location, response, socket->getServer());
	else
		_handle_file(uri, response, socket->getServer());
}

void	HttpRequestGet::generateResponse( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	if (!_has_cgi)
		response.fillHeader();
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket, bool &end)
{
	(void)socket;
	(void)fd;
	end = true;
}

bool	HttpRequestGet::hasCgi() const
{
	return _has_cgi;
}

Cgi	*HttpRequestGet::getCgi()
{
	return &_cgi;
}
