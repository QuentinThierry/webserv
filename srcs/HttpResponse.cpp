#include "HttpResponse.class.hpp"
#include "Cluster.class.hpp"

HttpResponse::HttpResponse( HttpResponse const & model)
{
	*this = model;
}

HttpResponse::HttpResponse( it_version const & version)
{
	_version = version;
	_status_code = HTTP_200;
	_fileOpen = false;
	_fields.push_back(HttpField("Connection", "close"));
	_fields.push_back(HttpField("Server", SERVER_NAME));
}


HttpResponse & HttpResponse::operator=(HttpResponse const & model )
{
	if (this != &model)
	{
		_version = model._version;
		_status_code = model._status_code;
		_fields = model._fields;
		_body = model._body;
	}
	return *this;
}

HttpResponse::~HttpResponse( void )
{
	//TODO
}

HttpResponse::HttpResponse( void )
{
	//TODO
}

uint32_t	HttpResponse::statusCodeToInt() const
{
	switch (_status_code)
	{
		case HTTP_307:
			return 307;
		case HTTP_308:
			return 308;
		case HTTP_400:
			return 400;
		case HTTP_401:
			return 401;
		case HTTP_402:
			return 402;
		case HTTP_403:
			return 403;
		case HTTP_404:
			return 404;
		case HTTP_405:
			return 405;
		case HTTP_406:
			return 406;
		case HTTP_407:
			return 407;
		case HTTP_408:
			return 408;
		case HTTP_409:
			return 409;
		case HTTP_410:
			return 410;
		case HTTP_411:
			return 411;
		case HTTP_412:
			return 412;
		case HTTP_413:
			return 413;
		case HTTP_414:
			return 414;
		case HTTP_415:
			return 415;
		case HTTP_416:
			return 416;
		case HTTP_417:
			return 417;
		case HTTP_418:
			return 418;
		case HTTP_421:
			return 421;
		case HTTP_422:
			return 422;
		case HTTP_423:
			return 423;
		case HTTP_424:
			return 424;
		case HTTP_425:
			return 425;
		case HTTP_426:
			return 426;
		case HTTP_428:
			return 428;
		case HTTP_429:
			return 429;
		case HTTP_431:
			return 431;
		case HTTP_451:
			return 451;
		case HTTP_500:
			return 500;
		case HTTP_501:
			return 501;
		case HTTP_502:
			return 502;
		case HTTP_503:
			return 503;
		case HTTP_504:
			return 504;
		case HTTP_505:
			return 505;
		case HTTP_506:
			return 506;
		case HTTP_507:
			return 507;
		case HTTP_508:
			return 508;
		case HTTP_510:
			return 510;
		case HTTP_511:
			return 511;
		default:
			throw (ExceptionUnknownStatusCode());
	}
}

void	HttpResponse::setStatusCode(e_status_code code)
{
	_status_code = code;
}


bool	HttpResponse::handle_redirect(Location const & location)
{
	if (location.getHasRedirect())
	{
		_status_code = location.getRedirect().first;
		_fields.push_back(HttpField("Location", location.getRedirect().second));
		return true;
	}
	return false;
}

void		HttpResponse::fillHeader()
{
	_header.clear();
	_header = *_version + " " + ft_itoa(statusCodeToInt()) + " "
			+ get_error_reason_phrase(_status_code) + "\r\n";
	for (unsigned int i = 0; i < _fields.size(); i++)
		_header += _fields.at(i).getFields();
	_header += "\r\n";
}

e_status HttpResponse::openFstream(std::string filename)
{
	_bodyFile.open(filename.c_str());
	if (_bodyFile.is_open() == false)
	{
		return FAILURE;
	}
	_fileOpen = true;
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0)
	{
		_fileOpen = false;
		_bodyFile.close();
		return FAILURE;
	}
	_fields.push_back(HttpField("Content-Length", ft_itoa(buffer.st_size)));
	return SUCCESS;
}

bool	HttpResponse::checkFieldExistence(std::string const & field_name) const
{
	std::vector<HttpField>::const_iterator it;

	for (it = _fields.begin(); it != _fields.end(); ++it)
	{
		if (it->getName() == field_name)
			return (true);
	}
	return (false);
}

void	HttpResponse::addAllowMethod(std::vector<std::string> const &method)
{
	HttpField res("Allow", method);
	_fields.push_back(res);
}


void HttpResponse::_removeField(std::string const &name)
{
	std::vector<HttpField>::iterator it;

	for (it = _fields.begin(); it != _fields.end(); ++it)
	{
		if (it->getName() == name)
		{
			_fields.erase(it);
			return;
		}
	}
}

void	HttpResponse::generateErrorResponse(e_status_code status, Server const & server)
{
	_version = --g_http_versions.end(); //!temporaire
	_status_code = status;
	if (!checkFieldExistence("Connection"))
		_fields.push_back(HttpField("Connection", "close"));
	if (!checkFieldExistence("Server"))
		_fields.push_back(HttpField("Server", SERVER_NAME));
	if (checkFieldExistence("Content-Length"))
		_removeField("Content-Length");
	if (_fileOpen == true)
	{
		_fileOpen = false;
		_bodyFile.close();
	}
	_body.clear();
	std::string path = server.getErrorPath(statusCodeToInt());
	openFstream(path);
}


void HttpResponse::writeResponse(int fd, Cluster &cluster)
{
	std::string buffer;

	if (_fileOpen)
	{
		char tmp[SIZE_WRITE + 1] = {0};
		_bodyFile.read(tmp, SIZE_WRITE);
		if (_bodyFile.eof() || _bodyFile.fail())
		{
			_fileOpen = false;
			_bodyFile.close();
		}
		buffer = tmp;
	}
	else if (!_header.empty())
	{
		buffer = _header;
		_header.clear();
	}
	else
	{
		buffer = _body;
		_body.clear();
	}
	int ret = write(fd, buffer.c_str(), buffer.size());
	if (ret == -1 || ret == 0)
	{
		if (_fileOpen == true)
			_bodyFile.close();
		cluster.closeConnection(fd);
	}
	if (_header.empty() && _body.empty() && _fileOpen == false)
	{
		if (_fileOpen == true)
			_bodyFile.close();
		cluster.closeConnection(fd);
	}
}