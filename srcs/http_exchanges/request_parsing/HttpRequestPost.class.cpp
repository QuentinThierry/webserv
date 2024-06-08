#include "HttpExchange.class.hpp"
#include "HttpRequestPost.class.hpp"
#include "utils.hpp"

std::vector<std::string> HttpRequestPost::_busyFile = std::vector<std::string>();

HttpRequestPost::HttpRequestPost (std::string const & str_request, Cluster & cluster)
	throw (ExceptionHttpStatusCode)
	: HttpRequest(), _cgi(cluster)
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_SSTREAM_FAIL);
	HttpRequest::init(stream_request);

	if (*getMethod() != "POST")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_WRONG_METHOD);
	
	if (empty_sstream_in_string(_body, stream_request)== FAILURE)
			throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_SSTREAM_FAIL);
		
	_read_size = _body.size();

	_content_length = 0;
	_chunk_read_size = 0;

	_has_size_chunk = false;
	_chunk_body_flags = false;
	_content_length_flags = false;
	_has_cgi = false;
}

HttpRequestPost::HttpRequestPost ( HttpRequestPost const & model)
	: HttpRequest(model)
{
	_filename = model._filename;

	_content_length = model._content_length;
	_chunk_read_size = model._chunk_read_size;
	_read_size = model._read_size;

	_chunk_body_flags = model._chunk_body_flags;
	_content_length_flags = model._content_length_flags;
	_has_size_chunk = model._has_size_chunk;

	_cgi = model._cgi;
	_has_cgi = model._has_cgi;
}

HttpRequestPost::HttpRequestPost( void )
{
	_content_length = 0;
	_chunk_read_size = 0;
	_read_size = 0;

	_chunk_body_flags = false;
	_content_length_flags = false;
	_has_size_chunk = false;

	_has_cgi = false;
}

HttpRequestPost & HttpRequestPost::operator= (HttpRequestPost const & model)
{
	if (&model != this)
	{
		HttpRequest::operator=(model);
		_filename = model._filename;

		_chunk_read_size = model._chunk_read_size;
		_content_length = model._content_length;
		_read_size = model._read_size;

		_chunk_body_flags = model._chunk_body_flags;
		_content_length_flags = model._content_length_flags;
		_has_size_chunk = model._has_size_chunk;

		_cgi = model._cgi;
		_has_cgi = model._has_cgi;
	}
	return (*this);
}

HttpRequestPost::~HttpRequestPost( void )
{
	std::vector<std::string>::iterator pos = find(_busyFile.begin(),
				_busyFile.end(), _filename);
	if (pos != _busyFile.end())
	{
		if (_file.is_open())
		{
			_file.close();
			remove(_filename.c_str());
		}
		_busyFile.erase(pos);
	}
}

static void	trim_filename(std::string & filename)
{
	bool has_slash = false;

	for (std::string::iterator it = filename.begin(); it != filename.end(); it++)
	{
		if (*it == '/')
		{
			if (has_slash)
			{
				it = filename.erase(it);
				it--;
			}
			else
				has_slash = true;
		}
		else if (has_slash)
			has_slash = false;
	}
}

bool	HttpRequestPost::isBusyFile(std::string filename)
{
	trim_filename(filename);
	for (std::vector<std::string>::iterator it = _busyFile.begin();
			it != _busyFile.end(); it++)
	{
		if (filename == *it)
			return true;
	}
	return false;
}

bool	HttpRequestPost::hasCgi() const {return _has_cgi;}

void	HttpRequestPost::setCgi(bool has_cgi) {_has_cgi = has_cgi;}

Cgi		*HttpRequestPost::getCgi() {return &_cgi;}

bool	HttpRequestPost::hasContentLength() const {return _content_length_flags;}

bool	HttpRequestPost::hasBody() const
{
	return (_chunk_body_flags == true || _content_length_flags == true);
}

void	HttpRequestPost::_closeFile()
{
	_file.close();
	std::vector<std::string>::iterator pos = find(_busyFile.begin(),
			_busyFile.end(), _filename);
	if (pos != _busyFile.end())
		_busyFile.erase(pos);
}

void	HttpRequestPost::_openFile()
{
	_file.open(_filename.c_str());
	if (!_file.is_open())
		throw ExceptionHttpStatusCode(HTTP_500);
	trim_filename(_filename);
	_busyFile.push_back(_filename);
}

void HttpRequestPost::_processBodyContentLength(bool &end)
{
	if (_content_length == 0)
	{
		if (_has_cgi)
			_cgi.endWrite();
		else
			_closeFile();
		end = true;
		return ;
	}
	if (_body.empty())
		return ;
	if (_has_cgi)
	{
		if (_cgi.write(_body) == -1)
			throw_http_err_with_log(HTTP_500, "ERROR: fail to write in cgi");
	}
	else
	{
		_file.write(_body.c_str(), _body.size());
		if (!_file.good())
			throw_http_err_with_log(HTTP_500, "ERROR: fail to write in socket");
	}
	if (_read_size == _content_length)
	{
		if (_has_cgi)
			_cgi.endWrite();
		else
			_closeFile();
		end = true;
	}
	_body.clear();
}

void	HttpRequestPost::_parseChunkSize()
{
	std::string number;
	_content_length = 0;
	size_t pos = _body.find_first_not_of("0123456789abcdefABCDEF");
	if (pos != std::string::npos && pos != 0)
		number = _body.substr(0, pos);
	else
		return ;
	bool error = false;
	_content_length = str_to_hex(number, error);
	if (error == true)
		throw_http_err_with_log(HTTP_413, "ERROR: chunk size too long for transfer-encoding=chuncked");

	if (pos + 1 >= _body.size())
		return ;
	if (!(_body[pos] == '\r' && _body[pos + 1] == '\n'))
		throw_http_err_with_log(HTTP_400, "ERROR: bad syntax for transfer-encoding=chuncked");
	_has_size_chunk = true;
	_body = _body.substr(pos + 2, _body.size() - (pos + 2));
}

bool	HttpRequestPost::_parseChunkBody()
{
	uint64_t write_size = _body.size();

	if (_chunk_read_size + _body.size() > _content_length)
		write_size = _content_length - _chunk_read_size;
	if (_has_cgi && write_size != 0)
	{
		if (_cgi.write(_body.substr(0, write_size)) == -1)
			throw_http_err_with_log(HTTP_500, "ERROR: fail to write in cgi");
	}
	else if (write_size != 0)
	{
		_file.write(_body.c_str(), write_size);
		if (!_file.good())
			throw_http_err_with_log(HTTP_500, "ERROR: fail to write in socket");
	};
	_chunk_read_size += write_size;
	if (write_size == _body.size())
		_body.clear();
	else
		_body = _body.substr(write_size, _body.size() - (write_size));
	if (_body.size() < 2)
		return false;
	if (_body[0] == '\r' && _body[1] == '\n')
	{
		_body = _body.substr(2, _body.size() - 2);
		_has_size_chunk = false;
		_chunk_read_size = 0;
	}
	else
	{
		throw_http_err_with_log(HTTP_400, "ERROR: bad syntax for transfer-encoding=chunked");
	}
	return true;
}

void	HttpRequestPost::processBody(bool &end)
{
	if (_content_length_flags)
		_processBodyContentLength(end);
	else if (_chunk_body_flags && !_body.empty())
	{
		if (!_has_size_chunk)
		{
			_parseChunkSize();
			if (_has_size_chunk == false)
				return;
			if (_content_length == 0)
			{
				if (_has_cgi)
					_cgi.endWrite();
				else
					_closeFile();
				end = true;
				return ;
			}
		}
		if (_has_size_chunk && !_body.empty())
		{
			if (!_parseChunkBody())
				return ;
		}
		if (!_body.empty() && !end && !_has_cgi)
			processBody(end);
	}
}

uint64_t	HttpRequestPost::_getSizeToReadBody(uint64_t max_boby_client) const
{
	if (_content_length_flags == true && _read_size + READ_SIZE > _content_length)
		return _content_length - _read_size;
	else if (_read_size + READ_SIZE > max_boby_client)
		return max_boby_client - _read_size;
	else
		return READ_SIZE;
}

void	HttpRequestPost::readBody(int fd, Socket const * const socket, bool &end)
{
	end = false;
	char buffer[READ_SIZE + 1] = {0};
	int read_size = _getSizeToReadBody(socket->getServer().getClientMaxBodySize());
	if (read_size <= 0)
		throw_http_err_with_log(HTTP_413, "ERROR: body too long in request");
	int ret = read(fd, buffer, read_size);
	if (ret == -1 || ret == 0)
	{
		if (ret == -1)
			protected_write_log(error_message_server(socket->getServer(),
					std::string("ERROR: read() ") + std::strerror(errno) + "at"));
		else
			protected_write_log(error_message_server(socket->getServer(),
				std::string("ERROR: read() end of file before the end of the body at")));
		throw ExceptionHttpStatusCode(HTTP_500);
	}
	_body += std::string(buffer, ret);
	_read_size += ret;
	if (!_has_cgi)
		processBody(end);
}

void	HttpRequestPost::_handleCgi(CgiLocation const & cgi_location, Server const &server)
{
	_cgi.exec(cgi_location.getExecPath(), cgi_location.getRootPath() + getTarget(), *this, server);
}

void	HttpRequestPost::_setBodyReadType(uint64_t maxClientBody)
{
	if (checkFieldExistence("Transfer-Encoding") == true)
	{
		std::vector<std::string> const &encoding = getFieldValue("Transfer-Encoding");
		if (encoding.size() != 1 || encoding[0] != "chunked")
			throw_http_err_with_log(HTTP_501, "ERROR: transfer-encoding other than chuncked is not supported");
		_chunk_body_flags = true;
		return ;
	}
	if (checkFieldExistence("Content-Length") == true)
	{
		std::vector<std::string> const &content_length = getFieldValue("Content-Length");
		if (content_length.size() != 1)
			throw_http_err_with_log(HTTP_400, "ERROR: invalid content-length field");
		e_status error = SUCCESS;
		_content_length = ft_atoi(content_length[0], error);
		if (error == FAILURE)
			throw_http_err_with_log(HTTP_400, "ERROR: invalid content-length");
		if (_content_length > maxClientBody)
			throw_http_err_with_log(HTTP_413, "ERROR: body too long in request");
		_content_length_flags = true;
		return ;
	}
}

void	HttpRequestPost::processHeader(Socket const * const socket)
{
	Location location = socket->getServer().searchLocation(getTarget());
	if (location.getHasRedirect())
		return ;
	if (isAcceptedMethod(location) == false)
		return ;
	if (!location.getCanUpload())
		throw_http_err_with_log(HTTP_403, "ERROR: can not upload body");
	_filename = getUri(location.getRootPath());
	_setBodyReadType(socket->getServer().getClientMaxBodySize());
	CgiLocation cgi_location;
	_has_cgi = socket->getServer().searchCgiLocation(_filename, cgi_location);
	if (_has_cgi)
		return _handleCgi(cgi_location, socket->getServer());
	if (access(_filename.c_str(), F_OK) == 0)
		throw_http_err_with_log(HTTP_409, "ERROR: file already exist");
	else if (errno == EACCES)
		throw_http_err_with_log(HTTP_403, "ERROR: don't have permission to create new file");
	_openFile();
	bool end = false;
	processBody(end);
	if (end)
	{
		_chunk_body_flags = false;
		_content_length_flags = false;
	}
}

void	HttpRequestPost::_initResponse( Socket const * const socket,
				HttpResponse &response )
{
	response.setVersion(getVersion());
	response.setStatusCode(HTTP_201);

	Location location = socket->getServer().searchLocation(getTarget());
	if (response.handle_redirect(location))
		return ;
	if (isAcceptedMethod(location) == false)
	{
		response.addAllowMethod(location.getMethods());
		throw_http_err_with_log(HTTP_405, "ERROR: method not allowed");
	}
	if (!_has_cgi)
		response.addField("Content-Length", "0");
}

void	HttpRequestPost::generateResponse( Socket const * const socket,
				HttpResponse &response )
{
	_initResponse(socket, response);
	if (!_has_cgi)
		response.fillHeader();
}
