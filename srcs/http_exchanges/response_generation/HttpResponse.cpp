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
	_fields.push_back(HttpField("Connection", "close"));
	_fields.push_back(HttpField("Server", SERVER_NAME));
	_fileOpen = false;
}

//! Can't copy open file(_bodyFile) during the operation
HttpResponse & HttpResponse::operator=(HttpResponse const & model )
{
	if (this != &model)
	{
		std::cout <<"COPY RESPONSE WARNING\n";
		_version = model._version;
		_status_code = model._status_code;
		_fields = model._fields;
		_header = model._header;

		_body = model._body;
		// _bodyFile = model._bodyFile;
		_fileOpen = false;
	}
	return *this;
}

HttpResponse::~HttpResponse( void )
{
	//TODO
}

HttpResponse::HttpResponse( void )
{
	_version = --g_http_versions.end();
	_status_code = HTTP_200;
	_fields.push_back(HttpField("Connection", "close"));
	_fields.push_back(HttpField("Server", SERVER_NAME));
	_fileOpen = false;
}

void	HttpResponse::setVersion(it_version version)
{
	_version = version;
}

void	HttpResponse::setStatusCode(e_status_code code)
{
	_status_code = code;
}

void	HttpResponse::addField(std::string name, std::string value)
{
	HttpField tmp(name, value);
	_fields.push_back(tmp);
}

bool	HttpResponse::handle_redirect(Location const & location)
{
	if (location.getHasRedirect())
	{
		_status_code = intToStatusCode(location.getRedirect().first);
		_fields.push_back(HttpField("Location", location.getRedirect().second));
		return true;
	}
	return false;
}

void		HttpResponse::fillHeader()
{
	_header.clear();
	std::cout <<  *_version << " " << statusCodeToInt() << " "
			<< get_error_reason_phrase(_status_code) << "\r\n";
	_header = *_version + " " + ft_itoa(statusCodeToInt()) + " "
			+ get_error_reason_phrase(_status_code) + "\r\n";
	for (unsigned int i = 0; i < _fields.size(); i++)
		_header += _fields.at(i).getFields();
	_header += "\r\n";
}

static void _close_body_file(std::ifstream &response_bodyFile, bool &response_fileOpen)
{
	response_fileOpen = false;
	if (response_bodyFile.is_open())
		response_bodyFile.close();
}

static e_status_code	_openFileStream(std::string & filename,
	std::ifstream &dest, bool &open_status_flag)
{
	if (access(filename.c_str(), F_OK) == -1)
	{
		if (errno == ENOENT)
			return HTTP_404;
		else if (errno == EACCES)
			return HTTP_403;
		else
			return HTTP_500;
	}
	dest.open(filename.c_str());
	if (dest.is_open() == false)
		return HTTP_403;
	open_status_flag = true;
	return HTTP_200;
}

static e_status _set_content_length_field(std::string &filename,
	std::vector<HttpField> &response_fields)
{
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0)
		return (FAILURE);
	response_fields.push_back(HttpField("Content-Length", ft_itoa(buffer.st_size)));
	return (SUCCESS);
}

e_status_code HttpResponse::openBodyFileStream(std::string filename)
{
	e_status_code status;

	status = _openFileStream(filename, _bodyFile, _fileOpen);
	if (status != HTTP_200)
		return (status);

	if (_set_content_length_field(filename, _fields) == FAILURE)
	{
		_close_body_file(_bodyFile, _fileOpen);
		return HTTP_403;
	}
	
	return HTTP_200;
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
		_close_body_file(_bodyFile, _fileOpen);
	_body.clear();
	std::string path = server.getErrorPagePath(statusCodeToInt());
	if (path.empty())
		_fields.push_back(HttpField("Content-Length", "0"));
	openBodyFileStream(path);
	fillHeader();
}


void HttpResponse::writeResponse(int fd, Cluster &cluster)
{
	std::string buffer;

	if (_header.empty() == false)
	{
		buffer = _header;
		_header.clear();
	}
	else if (_fileOpen)
	{
		char tmp[SIZE_WRITE + 1] = {0};
		_bodyFile.read(tmp, SIZE_WRITE);
		if (_bodyFile.eof() || _bodyFile.fail())
			_close_body_file(_bodyFile, _fileOpen);
		buffer = tmp;
	}
	else
	{
		buffer = _body;
		_body.clear();
	}
	int ret = write(fd, buffer.c_str(), buffer.size());
	if (ret == -1 || ret == 0)
	{
		std::cout << "error\n";
		if (_fileOpen == true)
			_close_body_file(_bodyFile, _fileOpen);
		cluster.closeConnection(fd);
		return ;
	}
	if (_header.empty() && _body.empty() && _fileOpen == false)
	{
		std::cout << "end\n";
		if (_fileOpen == true) //pas possible 
			_bodyFile.close();
		cluster.closeConnection(fd);
	}
}

uint32_t	HttpResponse::statusCodeToInt() const
{
	switch (_status_code)
	{
		case HTTP_100:
			return 100;
		case HTTP_101:
			return 101;
		case HTTP_102:
			return 102;
		case HTTP_103:
			return 103;
		case HTTP_200:
			return 200;
		case HTTP_201:
			return 201;
		case HTTP_202:
			return 202;
		case HTTP_203:
			return 203;
		case HTTP_204:
			return 204;
		case HTTP_205:
			return 205;
		case HTTP_206:
			return 206;
		case HTTP_207:
			return 207;
		case HTTP_208:
			return 208;
		case HTTP_226:
			return 226;
		case HTTP_300:
			return 300;
		case HTTP_301:
			return 301;
		case HTTP_302:
			return 302;
		case HTTP_303:
			return 303;
		case HTTP_304:
			return 304;
		case HTTP_305:
			return 305;
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

e_status_code	HttpResponse::intToStatusCode(uint16_t number) const
{
	switch (number)
	{
		case 100:
			return HTTP_100;
		case 101:
			return HTTP_101;
		case 102:
			return HTTP_102;
		case 103:
			return HTTP_103;
		case 200:
			return HTTP_200;
		case 201:
			return HTTP_201;
		case 202:
			return HTTP_202;
		case 203:
			return HTTP_203;
		case 204:
			return HTTP_204;
		case 205:
			return HTTP_205;
		case 206:
			return HTTP_206;
		case 207:
			return HTTP_207;
		case 208:
			return HTTP_208;
		case 226:
			return HTTP_226;
		case 300:
			return HTTP_300;
		case 301:
			return HTTP_301;
		case 302:
			return HTTP_302;
		case 303:
			return HTTP_303;
		case 304:
			return HTTP_304;
		case 305:
			return HTTP_305;
		case 307:
			return HTTP_307;
		case 308:
			return HTTP_308;
		case 400:
			return HTTP_400;
		case 401:
			return HTTP_401;
		case 402:
			return HTTP_402;
		case 403:
			return HTTP_403;
		case 404:
			return HTTP_404;
		case 405:
			return HTTP_405;
		case 406:
			return HTTP_406;
		case 407:
			return HTTP_407;
		case 408:
			return HTTP_408;
		case 409:
			return HTTP_409;
		case 410:
			return HTTP_410;
		case 411:
			return HTTP_411;
		case 412:
			return HTTP_412;
		case 413:
			return HTTP_413;
		case 414:
			return HTTP_414;
		case 415:
			return HTTP_415;
		case 416:
			return HTTP_416;
		case 417:
			return HTTP_417;
		case 418:
			return HTTP_418;
		case 421:
			return HTTP_421;
		case 422:
			return HTTP_422;
		case 423:
			return HTTP_423;
		case 424:
			return HTTP_424;
		case 425:
			return HTTP_425;
		case 426:
			return HTTP_426;
		case 428:
			return HTTP_428;
		case 429:
			return HTTP_429;
		case 431:
			return HTTP_431;
		case 451:
			return HTTP_451;
		case 500:
			return HTTP_500;
		case 501:
			return HTTP_501;
		case 502:
			return HTTP_502;
		case 503:
			return HTTP_503;
		case 504:
			return HTTP_504;
		case 505:
			return HTTP_505;
		case 506:
			return HTTP_506;
		case 507:
			return HTTP_507;
		case 508:
			return HTTP_508;
		case 510:
			return HTTP_510;
		case 511:
			return HTTP_511;
		default:
			throw (ExceptionUnknownStatusCode());
	}
}