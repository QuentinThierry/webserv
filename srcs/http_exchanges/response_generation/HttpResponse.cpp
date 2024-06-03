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
	_write_size = 0;
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
	_write_size = 0;
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
		_write_size = model._write_size;
	}
	return *this;
}

HttpResponse::~HttpResponse( void )
{
	if (_bodyFile.is_open())
		_bodyFile.close();
}

std::string const	&HttpResponse::getBody(void) const {return (_body);}

bool	HttpResponse::is_response_ready() const {return _response_ready;}

void	HttpResponse::setVersion(it_version version) {_version = version;}

void	HttpResponse::setStatusCode(e_status_code code){_status_code = code;}

void	HttpResponse::setEndOfFile() {_end_of_file_flag = true;}

void	HttpResponse::addBodyContent(std::string str) {_body += str;}

void	HttpResponse::addField(std::string name, std::string value)
{
	HttpField tmp(name, value);
	_fields.push_back(tmp);
}

void	HttpResponse::addAllowMethod(std::vector<std::string> const &method)
{
	HttpField res("Allow", method);
	_fields.push_back(res);
}

void		HttpResponse::setBody(std::string &body_content)
{
	_body = body_content;
	_content_length = _body.size();
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

void	HttpResponse::fillHeader()
{
	_header.clear();
	_header = *_version + " ";
	if (_custom_status.empty()) 
		_header += ft_itoa(status_code_to_int(_status_code)) + " "
			+ get_error_reason_phrase(_status_code);
	else
	 	_header += _custom_status;
	_header += "\r\n";
	for (unsigned int i = 0; i < _fields.size(); i++)
		_header += _fields.at(i).getFields();
	_header += "\r\n";
	_response_ready = true;
	displayHeader();
}

void	HttpResponse::displayHeader()
{
	std::cout << "------------ HttpResponse ------------" << std::endl;
	std::cout << *_version << " ";
	if (_custom_status.empty()) 
		std::cout << ft_itoa(status_code_to_int(_status_code)) << " "
			<< get_error_reason_phrase(_status_code);
	else
	 	std::cout << _custom_status;
	std::cout << std::endl;
	for (unsigned int i = 0; i < _fields.size(); i++)
		_fields.at(i).display_field();
}

static void	_close_body_file(std::ifstream &response_bodyFile, bool &response_fileOpen)
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

static e_status	_extract_content_length(std::vector<HttpField> &response_fields,
					uint64_t &value_dest, bool &has_content_length)
{
	std::vector<std::string>	field_values;
	e_status					atoi_error;

	atoi_error = SUCCESS;
	if (HttpField::extract_field(response_fields, "Content-Length", field_values) == SUCCESS)
	{
		std::cout << "Content-Length found"<<std::endl <<std::endl;//
		if (field_values.size() != 1)
		{
			// ;//TODO error code 413 
			protected_write(g_err_log_fd, "Warning: cgi: incorrect content length size");
			return (FAILURE);
		}
		value_dest = ft_atoi(field_values.at(0), atoi_error);
		if (atoi_error == FAILURE )
		{
			// ;//TODO error code 413 
			protected_write(g_err_log_fd, "Warning: cgi: incorrect content length format");
			return (FAILURE);
		}
		response_fields.push_back(HttpField("Content-Length", field_values));
		has_content_length = true;
	}
	else
		value_dest = 0;
	return (SUCCESS);
}

static e_status	_extract_status_code(std::vector<HttpField> &response_fields,
					e_status_code &response_status, std::string &custom_status)
{
	std::vector<std::string>	field_values;

	if (HttpField::extract_field(response_fields, "Status", field_values) == SUCCESS)
	{
		// std::cout << std::endl << "Status found" << std::endl << std::endl;//
		if (field_values.size() == 0)
		{
			response_status = HTTP_502;//TODO error code (? 502) 
			protected_write(g_err_log_fd, "cgi: empty status send in response");
			return (FAILURE);
		}
		custom_status += field_values.at(0);
		for (std::vector<std::string>::iterator it = field_values.begin() + 1;
				it != field_values.end(); ++it)
			custom_status += "," + *it;
	}
	else
		response_status = HTTP_200;
	return (SUCCESS);
}

void	HttpResponse::_extract_cgi_fields_data( void )
{
	if (_extract_content_length(_fields, _content_length, _content_length_flag) == FAILURE
		|| _extract_status_code(_fields, _status_code, _custom_status) == FAILURE)
		return ;//TODO
}

void	HttpResponse::parseCgiHeader(std::string header) throw(ExceptionHttpStatusCode)
{
	std::cout << header << std::endl;

	std::stringstream header_stream(header);

	HttpField::fill_fields(header_stream, _fields);

	if (empty_sstream_in_string(_body, header_stream)== SUCCESS)
		_extract_cgi_fields_data();
	else //! comportement a confirmer, faire comme generateErrorResponse ?
	{
		_fields.clear(); 
		_fields.push_back(HttpField("Connection", "close"));
		_fields.push_back(HttpField("Server", SERVER_NAME));
		_status_code = HTTP_500;
		return ;
	}
	if (!_content_length_flag)
		_fields.push_back(HttpField("Transfer-Encoding", "chunked"));
	fillHeader();
}

static std::string _get_img_path(e_status_code error_code)
{
	char const *error_path[] = RANDOMIZED_ERROR_PAGE_PATHS;

	switch (error_code) {
		case HTTP_404:
			return (ERROR_404_PATH);
		case HTTP_403:
			return (ERROR_403_PATH);
		case HTTP_418:
			return (ERROR_418_PATH);
		case HTTP_500:
			return (ERROR_500_PATH);
		default:
			unsigned int i = std::rand() % (sizeof(error_path) / sizeof(char **));
			return (error_path[i]);
	}
}

void	HttpResponse::_generateErrorPageBody(e_status_code error_code)
{
	std::string error_message = ft_itoa(error_code) + " " + get_error_reason_phrase(error_code);
	std::string img_path = _get_img_path(error_code);
	_body =
	"<!DOCTYPE html>\r\n"
	"<html>\r\n"
	"	<head>\r\n"
	"		<title>\r\n" +
				error_message +
	"		</title>\r\n"
	"	</head>\r\n"
	"	<body>\r\n"
	"		<center>\r\n"
	"			<h1>\r\n" +
				error_message +
	"			</h1>\r\n"
	"		</center>\r\n"
	"		<hr>\r\n"
	"		<center>\r\n"
	"			webserv\r\n"
	"		</center>\r\n"
	"		<center>\r\n"
	"			<img src='" + img_path + "' style='max-height:500px; max-width:500px; height: 50%; width: 100%; object-fit: contain;'>\r\n" +
	"		</center>\r\n"
	"	</body>\r\n"
	"</html>\r\n";
	_fields.push_back(HttpField("Content-Length", ft_itoa(_body.size())));
	_content_length_flag = true;
	_content_length = _body.size();
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
	if (path.empty())
		_generateErrorPageBody(_status_code);
	else if (openBodyFileStream(path) != HTTP_200)
		_fields.push_back(HttpField("Content-Length", "0"));
	fillHeader();
}

bool	HttpResponse::_checkEndCgi(bool has_cgi) const
{
	if (has_cgi)
	{
		if (_content_length_flag && _content_length == _write_size)
			return true;
		else if (!_content_length_flag && _end_of_file_flag)
			return true;
	}
	return false;
}

void	HttpResponse::_chunkResponse()
{
	int size = _body.size();

	_body = ft_itoa(size) + "\r\n" + _body + "\r\n";
	if (_checkEndCgi(true))
		_body += "0\r\n";
}

static ssize_t	send_header(int fd, std::string &header)
{
	ssize_t ret = send(fd, header.c_str(), header.size(), MSG_NOSIGNAL);
	header.clear();
	return ret;
}

static ssize_t	send_body_file(int fd, std::ifstream &file, bool &is_open)
{
	char tmp[SIZE_WRITE + 1] = {0};
	file.read(tmp, SIZE_WRITE);
	if (file.eof() || file.fail())
		_close_body_file(file, is_open);
	file.gcount();
	ssize_t ret = send(fd, tmp, file.gcount(), MSG_NOSIGNAL);
	return ret;
}

ssize_t	HttpResponse::_sendBodyString(int fd, bool has_cgi)
{
	if (has_cgi && !_content_length_flag)
		_chunkResponse();
	ssize_t ret = send(fd, _body.c_str(), _body.size(), MSG_NOSIGNAL);
	_write_size += ret; //!can send less then body.size()
	_body.clear();
	return ret;
}

void	HttpResponse::writeResponse(int fd, Cluster &cluster, bool has_cgi)
{
	int ret = 1;
	if (_header.empty() == false)
		ret = send_header(fd, _header);
	else if (_fileOpen)
		ret = send_body_file(fd, _bodyFile, _fileOpen);
	else if (!_body.empty())
		ret = _sendBodyString(fd, has_cgi);
	if (ret == -1 || ret == 0)
	{
		std::cout << "error\n";
		if (_fileOpen == true)
			_close_body_file(_bodyFile, _fileOpen);
		cluster.closeConnection(fd);
		return ;
	}
	if ((!has_cgi && _header.empty() && _body.empty()
			&& _fileOpen == false && _content_length == _write_size)
		|| _checkEndCgi(has_cgi))
	{
		std::cout << "end\n";
		cluster.closeConnection(fd);
	}
}