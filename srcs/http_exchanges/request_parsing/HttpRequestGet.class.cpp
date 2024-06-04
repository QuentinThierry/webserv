#include "HttpRequestGet.class.hpp"

HttpRequestGet::HttpRequestGet (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest(), _cgi(), _has_cgi(false)
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPGET_SSTREAM_FAIL);
	HttpRequest::init(stream_request);
	if (*getMethod() != "GET")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPGET_WRONG_METHOD);
}

HttpRequestGet::HttpRequestGet ( HttpRequestGet const & model)
	: HttpRequest(model), _cgi(model._cgi), _has_cgi(model._has_cgi)
{}

HttpRequestGet::HttpRequestGet( void ): _cgi(), _has_cgi(false)
{}

HttpRequestGet & HttpRequestGet::operator= (HttpRequestGet const & model)
{
	if (&model != this)
	{
		HttpRequest::operator=(model);
		_cgi = model._cgi;
		_has_cgi = model._has_cgi;
	}
	return (*this);
}

HttpRequestGet::~HttpRequestGet( void )
{}

void	HttpRequestGet::processHeader( Socket const * const socket ) {(void)socket;}

bool	HttpRequestGet::hasBody() const {return (false);}

bool	HttpRequestGet::hasCgi() const {return _has_cgi;}

void	HttpRequestGet::setCgi(bool has_cgi) {_has_cgi = has_cgi;}

Cgi		*HttpRequestGet::getCgi() {return &_cgi;}

std::string const & HttpRequestGet::getQueryString() const {return _query_string;}

void	HttpRequestGet::readBody(int fd, Socket const * const socket, bool &end)
{
	(void)socket;
	(void)fd;
	end = true;
}

static void	_add_content_type_field(HttpResponse & response, std::string const &target_uri)
{
	response.addField("Content-Type", get_MIME_type(target_uri));
}

std::string	HttpRequestGet::getUri(std::string root)
{
	std::string uri = root + getTarget();
	size_t pos = uri.find_first_of('?');
	if (pos != std::string::npos)
	{
		_query_string = uri.substr(pos + 1, uri.size());
		uri = uri.substr(0, pos);
	}
	return (uri);
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
		_add_content_type_field(response, ".html");
}

e_status	HttpRequestGet::_handle_index_file(std::string & uri, Location const & location,
				HttpResponse & response,  Server const & server)
{
	if (location.updateUriToIndex(uri) == SUCCESS)
	{
		_handleFile(uri, response, server);
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

void	HttpRequestGet::_redirectDirectory(HttpResponse & response)
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
		_handle_Autoindex(location.getRootPath(), getTarget(), response);
		return ;
	}
	throw_http_err_with_log(HTTP_403, "ERROR: no such file or directory");
}

void	HttpRequestGet::_handleCgi(Server const & server,
						CgiLocation const &cgi_location)
{
	std::string uri = getUri(cgi_location.getRootPath());
	std::cout << uri <<std::endl;
	std::cout << _query_string <<std::endl;
	_cgi.exec(cgi_location.getExecPath(), uri, *this, server);
}

void	HttpRequestGet::_handleFile(std::string & uri, HttpResponse & response,
								Server const & server)
{
	CgiLocation cgi_location;
	_has_cgi = server.searchCgiLocation(uri, cgi_location);
	if (_has_cgi)
		return _handleCgi(server, cgi_location);
	e_status_code error_code = response.openBodyFileStream(uri);
	if (error_code != HTTP_200)
		throw ExceptionHttpStatusCode(error_code);
	_add_content_type_field(response, uri);
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
		throw_http_err_with_log(HTTP_405, "ERROR: method not allowed");
	}
	std::string uri = getUri(location.getRootPath());
	if (is_accessible_directory(uri.c_str()))
		_handleDirectory(uri, location, response, socket->getServer());
	else
		_handleFile(uri, response, socket->getServer());
}

void	HttpRequestGet::generateResponse( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	if (!_has_cgi)
		response.fillHeader();
}
