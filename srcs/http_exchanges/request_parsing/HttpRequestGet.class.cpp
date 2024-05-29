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

static void	_handle_file(std::string & uri, HttpResponse & response)
{
	e_status_code error_code = response.openBodyFileStream(uri);
	if (error_code != HTTP_200)
		throw ExceptionHttpStatusCode(error_code);
}

static e_status	_handle_index_file(std::string const & target, Location const & location,
				HttpResponse & response)
{
	std::string	uri;

	uri = getUri(location.getRootPath(), target);
	if (location.updateUriToIndex(uri) == SUCCESS)
	{
		_handle_file(uri, response);
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

static void	_add_autoindex_body(HttpResponse & response, Autoindex & index)
{
	std::string body_content;

	body_content = index.generateAutoIndexBody();
	response.setBody(body_content);
}

static void _add_content_length_field(HttpResponse & response)
{
	std::string	field_name;
	std::string length;

	field_name = "Content-Length";
	length = ft_itoa(response.getBody().length());
	response.addField(field_name, length);
}

static void	_handle_Autoindex(std::string const & location_root,
				std::string const & target, HttpResponse & response)
{
		Autoindex index(location_root, target);

		_add_autoindex_body(response, index);
		_add_content_length_field(response);		

		response.fillHeader();
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

void	HttpRequestGet::_handleDirectory(std::string const & target,
				Location const & location, HttpResponse & response)
{
	if (*(target.end() - 1) != '/')
		_redirectDirectory(response);
	else if (location.hasDefaultIndex()
			&& _handle_index_file(target, location, response) == SUCCESS)
		return;
	else if (location.getHasAutoindex())
	{
		_handle_Autoindex(location.getRootPath(), target, response);
		return ;
	}
	throw ExceptionHttpStatusCode(HTTP_403);
}

void	HttpRequestGet::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());

	Location location = socket->getServer().searchLocation(getTarget());
	//get location cgi if cgi
	if (response.handle_redirect(location))
		return ;
	if (isAcceptedMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		throw ExceptionHttpStatusCode(HTTP_405);
	}
	
	std::string uri = getUri(location.getRootPath(), getTarget());
	if (is_accessible_directory(uri.c_str()))
		_handleDirectory(getTarget(), location, response);
	else
		_handle_file(uri, response);
}

void	HttpRequestGet::generateResponse( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	response.fillHeader();
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket, bool &end)
{
	(void)socket;
	(void)fd;
	end = true;
}
