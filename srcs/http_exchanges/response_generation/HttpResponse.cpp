#include "HttpResponse.class.hpp"
#include "HttpRequestPost.class.hpp"
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
	_response_ready = false;
	_content_length = 0;
	_content_length_flag = false;
	_end_of_file_flag = false;
	_read_size = 0;
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
		_response_ready = model._response_ready;
		_content_length = model._content_length;
		_content_length_flag = model._content_length_flag;
		_end_of_file_flag = model._end_of_file_flag;
		_read_size = model._read_size;
	}
	return *this;
}

HttpResponse::~HttpResponse( void )
{
	if (_bodyFile.is_open())
		_bodyFile.close();
}

HttpResponse::HttpResponse( void )
{
	_version = --g_http_versions.end();
	_status_code = HTTP_200;
	_fields.push_back(HttpField("Connection", "close"));
	_fields.push_back(HttpField("Server", SERVER_NAME));
	_fileOpen = false;
	_response_ready = false;
	_content_length = 0;
	_content_length_flag = false;
	_end_of_file_flag = false;
	_read_size = 0;
}

bool	HttpResponse::is_response_ready()
{
	return _response_ready;
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

void	HttpResponse::addBodyContent(std::string str)
{
	_body += str;
}

void		HttpResponse::setEndOfFile()
{
	_end_of_file_flag = true;
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
	_header = *_version + " " + ft_itoa(status_code_to_int(_status_code)) + " "
			+ get_error_reason_phrase(_status_code) + "\r\n";
	for (unsigned int i = 0; i < _fields.size(); i++)
		_header += _fields.at(i).getFields();
	_header += "\r\n";
	_response_ready = true;
	displayHeader();
}

void		HttpResponse::displayHeader()
{
	std::cout << "------------ HttpResponse ------------" << std::endl;
	std::cout << *_version << " " << ft_itoa(status_code_to_int(_status_code)) << " "
			<< get_error_reason_phrase(_status_code) << "\r\n";
	for (unsigned int i = 0; i < _fields.size(); i++)
		_fields.at(i).display_field();
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
		if (errno == ENOENT || errno == ENOTDIR)
			return HTTP_404;
		else if (errno == EACCES)
			return HTTP_403;
		return HTTP_500; //! not sure
	}
	if (HttpRequestPost::isBusyFile(filename))
		return HTTP_404;
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

const std::vector<std::string>	&HttpResponse::getFieldValue(
	std::string const & field_name) const throw(ExceptionHttpStatusCode)
{
	std::vector<HttpField>::const_iterator it;

	for (it = _fields.begin(); it != _fields.end(); ++it)
	{
		if (it->getName() == field_name)
			return (it->getValues());
	}
	throw(ExceptionHttpStatusCode(HTTP_500));
}

void	HttpResponse::addAllowMethod(std::vector<std::string> const &method)
{
	HttpField res("Allow", method);
	_fields.push_back(res);
}


static e_status	_extract_content_length(std::vector<HttpField> &response_fields,
					uint64_t &value_dest, bool &has_content_length)
{
	std::vector<std::string>	field_values;
	e_status					itoa_error;

	itoa_error = SUCCESS;
	if (HttpField::extract_field(response_fields, "Content-Length", field_values) == SUCCESS)
	{
		if (field_values.size() != 1)
		{
			// ;//TODO error code 413 
			return (FAILURE);
		}
		value_dest = ft_atoi(field_values.at(0), itoa_error);
		if (itoa_error == FAILURE )
		{
			// ;//TODO error code 413 
			return (FAILURE);
		}
		has_content_length = true;
	}
	else
		value_dest = 0;
	return (SUCCESS);
}

static e_status	_extract_status_code(std::vector<HttpField> &response_fields,
					e_status_code &response_status)
{
	std::vector<std::string>	field_values;
	e_status					itoa_error;
	uint64_t					status_code;

	itoa_error = SUCCESS;
	if (HttpField::extract_field(response_fields, "Status", field_values) == SUCCESS)
	{
		if (field_values.size() == 0)
		{
			// ;//TODO error code (? 413) 
			return (FAILURE);
		}
		status_code = ft_atoi(field_values.at(0), itoa_error);
		if (itoa_error == FAILURE )
		{
			// ;//TODO error code 413 
			return (FAILURE);
		}
		response_status = int_to_status_code(status_code);
	}
	else
		response_status = HTTP_200;
	return (SUCCESS);
}

void	HttpResponse::_extract_cgi_fields_data( void )
{
	if (_extract_content_length(_fields, _content_length, _content_length_flag) == FAILURE
		|| _extract_status_code(_fields, _status_code) == FAILURE)
		return ;//TODO
}

void HttpResponse::parseCgiHeader(std::string header) throw(ExceptionHttpStatusCode)
{
	std::cout << header << std::endl;

	std::stringstream header_stream(header);

	HttpField::fill_fields(header_stream, _fields);

	if (empty_sstream_in_string(_body, header_stream)== SUCCESS)
		_extract_cgi_fields_data();
	else //! comportement A confirmer
	{
		_fields.clear(); 
		_fields.push_back(HttpField("Connection", "close"));
		_fields.push_back(HttpField("Server", SERVER_NAME));
		_status_code = HTTP_500;
		return ;
	}
	if (!_content_length_flag)
		_fields.push_back(HttpField("Transfer-Encoding", "chunked"));
	_read_size = _body.size();
	fillHeader();
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
		HttpField::erase_field(_fields, "Content-Length");
	if (_fileOpen == true)
		_close_body_file(_bodyFile, _fileOpen);
	_body.clear();
	std::string path = server.getErrorPagePath(status_code_to_int(_status_code));
	if (path.empty() || openBodyFileStream(path) != HTTP_200)
		_fields.push_back(HttpField("Content-Length", "0"));
	fillHeader();
}

bool HttpResponse::_checkEndCgi(bool has_cgi) const
{
	if (has_cgi)
	{
		if (_content_length_flag && _content_length == _read_size)
			return true;
		else if (!_content_length_flag && _end_of_file_flag)
			return true;
	}
	return false;
}

void HttpResponse::_chunkResponse()
{
	int size = _body.size();

	_body = ft_itoa(size) + "\r\n" + _body + "\r\n";
	if (_checkEndCgi(true))
		_body += "0\r\n";
}

void HttpResponse::writeResponse(int fd, Cluster &cluster, bool has_cgi)
{
	int ret = 1;
	if (_header.empty() == false)
	{
		std::cout << "send header" << std::endl;
		ret = send(fd, _header.c_str(), _header.size(), MSG_NOSIGNAL);
		_header.clear();
	}
	else if (_fileOpen)
	{
		char tmp[SIZE_WRITE + 1] = {0};
		_bodyFile.read(tmp, SIZE_WRITE);
		if (_bodyFile.eof() || _bodyFile.fail())
			_close_body_file(_bodyFile, _fileOpen);
		_bodyFile.gcount();
		ret = send(fd, tmp, _bodyFile.gcount(), MSG_NOSIGNAL);
	}
	else if (!_body.empty())
	{
		if (has_cgi && !_content_length_flag)
			_chunkResponse();
		std::cout << "send body" << std::endl;
		ret = send(fd, _body.c_str(), _body.size(), MSG_NOSIGNAL);
		_read_size += ret; //!can send less then body.size()
		_body.clear();
	}
	if (ret == -1 || ret == 0)
	{
		std::cout << "error\n";
		if (_fileOpen == true)
			_close_body_file(_bodyFile, _fileOpen);
		cluster.closeConnection(fd);
		return ;
	}
	if ((!has_cgi && _header.empty() && _body.empty() && _fileOpen == false && _content_length == _read_size)
		|| _checkEndCgi(has_cgi))
	{
		std::cout << "has cgi:" << has_cgi << std::endl;
		std::cout << "content flags:" << _content_length_flag << std::endl;
		std::cout << "eof:" << _end_of_file_flag << std::endl;
		std::cout << "end\n";
		cluster.closeConnection(fd);
	}
}