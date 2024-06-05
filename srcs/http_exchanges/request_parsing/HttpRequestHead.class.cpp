#include "HttpRequestHead.class.hpp"

HttpRequestHead::HttpRequestHead (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest(), _cgi(), _has_cgi(false)
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPHEAD_SSTREAM_FAIL);
	HttpRequest::init(stream_request);
	if (*getMethod() != "HEAD")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPHEAD_WRONG_METHOD);
}

HttpRequestHead::HttpRequestHead ( HttpRequestHead const & model)
	: HttpRequest(model), _cgi(model._cgi), _has_cgi(model._has_cgi), _query_string(model._query_string)
{}

HttpRequestHead::HttpRequestHead( void ): _cgi(), _has_cgi(false)
{}

HttpRequestHead & HttpRequestHead::operator= (HttpRequestHead const & model)
{
	if (&model != this)
	{
		HttpRequest::operator=(model);
		_cgi = model._cgi;
		_has_cgi = model._has_cgi;
		_query_string = model._query_string;
	}
	return (*this);
}

HttpRequestHead::~HttpRequestHead( void )
{}

void	HttpRequestHead::processHeader( Socket const * const socket ) {(void)socket;}

bool	HttpRequestHead::hasBody() const {return (false);}

bool	HttpRequestHead::hasCgi() const {return _has_cgi;}

void	HttpRequestHead::setCgi(bool has_cgi) {_has_cgi = has_cgi;}

Cgi		*HttpRequestHead::getCgi() {return &_cgi;}

std::string const & HttpRequestHead::getQueryString() const {return _query_string;}

void	HttpRequestHead::readBody(int fd, Socket const * const socket, bool &end)
{
	(void)socket;
	(void)fd;
	end = true;
}

static void	_add_content_type_field(HttpResponse & response, std::string const &target_uri)
{
	response.addField("Content-Type", get_MIME_type(target_uri));
}

std::string	HttpRequestHead::getUri(std::string root)
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

static void _add_content_length_field(HttpResponse & response,size_t length)
{
	std::string	field_name;
	std::string length_str;

	field_name = "Content-Length";
	length_str = ft_itoa(length);
	response.addField(field_name, length_str);
}

static void	_handle_Autoindex(std::string const & location_root,
				std::string const & target, HttpResponse & response)
{
		Autoindex	index(location_root, target);

		_add_content_length_field(response, index.generateAutoIndexBody().length());
		_add_content_type_field(response, ".html");
}

e_status	HttpRequestHead::_handle_index_file(std::string & uri, Location const & location,
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

void	HttpRequestHead::_redirectDirectory(HttpResponse & response)
{
	std::string location = "http://";
	location += getFieldValue("Host")[0];
	location += getTarget();
	location += "/";
	response.addField("Location", location);
	throw ExceptionHttpStatusCode(HTTP_301);
}

void	HttpRequestHead::_handleDirectory(std::string & uri, Location const & location,
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

void	HttpRequestHead::_handleCgi(Server const & server,
						CgiLocation const &cgi_location)
{
	std::string uri = getUri(cgi_location.getRootPath());
	std::cout << uri <<std::endl;
	std::cout << _query_string <<std::endl;
	_cgi.exec(cgi_location.getExecPath(), uri, *this, server);
}

void	HttpRequestHead::_handleFile(std::string & uri, HttpResponse & response,
								Server const & server)
{
	CgiLocation cgi_location;
	_has_cgi = server.searchCgiLocation(uri, cgi_location);
	if (_has_cgi)
		return _handleCgi(server, cgi_location);
	e_status_code error_code = response.openBodyFileStream(uri);
	response.closeBodyFile();
	if (error_code != HTTP_200)
		throw ExceptionHttpStatusCode(error_code);
	_add_content_type_field(response, uri);
}

void	HttpRequestHead::_initResponse( Socket const * const socket, HttpResponse &response )
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
	_body.clear();
}

void	HttpRequestHead::generateResponse( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	if (!_has_cgi)
		response.fillHeader();
}
