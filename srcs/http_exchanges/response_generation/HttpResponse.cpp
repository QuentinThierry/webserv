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
		_status_code = int_to_status_code(location.getRedirect().first);
		_fields.push_back(HttpField("Location", location.getRedirect().second));
		return true;
	}
	return false;
}

void		HttpResponse::fillHeader()
{
	_header.clear();
	std::cout <<  *_version << " " << status_code_to_int(_status_code) << " "
			<< get_error_reason_phrase(_status_code) << "\r\n";
	_header = *_version + " " + ft_itoa(status_code_to_int(_status_code)) + " "
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
	std::string path = server.getErrorPagePath(status_code_to_int(_status_code));
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