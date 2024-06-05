#include "HttpExchange.class.hpp"
#include "HttpResponseStatus.hpp"
#include "HttpRequestPost.class.hpp"
#include "HttpRequestGet.class.hpp"
#include "HttpRequestDelete.class.hpp"
#include "HttpRequestHead.class.hpp"

#include "Cluster.class.hpp"
#include "utils.hpp"

#include <cstdlib>

HttpExchange::HttpExchange(Socket const &socket): _socket(&socket), _response()
{
	if (gettimeofday(&_accept_request_time, NULL) == -1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
				std::string("Error: ") + std::strerror(errno) + " at"));
		throw ExceptionHttpStatusCode(HTTP_500);
	}
	_buffer_read.clear();
	_request = NULL;
}

HttpExchange::HttpExchange(HttpExchange const &copy): _socket(copy._socket)
{
	*this = copy;
}

HttpExchange & HttpExchange::operator=(HttpExchange const &assign)
{
	if (this != &assign)
	{
		_buffer_read = assign._buffer_read;
		if (assign._request != NULL)
			_copyRequest(_findMethod(*(assign._request->getMethod())), assign._request);
		else
			_request = NULL;
		_response = assign._response;
		_accept_request_time.tv_sec = assign._accept_request_time.tv_sec;
		_accept_request_time.tv_usec = assign._accept_request_time.tv_usec;
	}
	return *this;
}

HttpExchange::~HttpExchange()
{
	if (_request != NULL)
		delete(_request);
}

void HttpExchange::_copyRequest(e_http_method method, HttpRequest const * request)
{
	switch (method)
	{
		case HEAD:
			_request = ::new HttpRequestHead(*((HttpRequestHead*)request));
			break;
		case GET:
			_request = ::new HttpRequestGet(*((HttpRequestGet*)request));
			break;
		case POST:
			_request = ::new HttpRequestPost(*((HttpRequestPost*)request));
			break;
		case DELETE:
			_request = ::new HttpRequestDelete(*((HttpRequestDelete*)request));
			break;
		default:
			protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
				std::string("Error: invalid request method from")));
			throw ExceptionHttpStatusCode(HTTP_500);
	}
}

HttpRequest const	&HttpExchange::getRequest() const {return *this->_request;}

HttpResponse const	&HttpExchange::getResponse() const {return this->_response;}

Socket const		&HttpExchange::getSocket() const {return *this->_socket;}

struct timeval const & HttpExchange::getAcceptRequestTime() const {return _accept_request_time;}

void	HttpExchange::_setRightSocket(Cluster const &cluster)
{
	if (_request->checkFieldExistence("Host") == false)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: Missing 'Host' variable in the http request from")));
		throw ExceptionHttpStatusCode(HTTP_400);
	}
	std::vector<std::string> const &host_name = _request->getFieldValue("Host");
	if (host_name.size() != 1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: Invalid format of 'Host' variable in the http request from")));
		throw ExceptionHttpStatusCode(HTTP_400);
	}
	Socket const *socket = cluster.getMatchingSocket(_socket->getFd(), host_name.at(0));
	if (socket != NULL)
		_socket = socket;
}

e_http_method	HttpExchange::_findMethod(std::string const & cmp)
{
	std::string method_name[4] = {"GET", "POST", "DELETE", "HEAD"};
	e_http_method method_value[4] = {GET, POST, DELETE, HEAD};

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < method_name[i].size(); j++)
		{
			if (method_name[i][j] != cmp[j])
				break;
			else if (j == method_name[i].size() - 1 && cmp[j + 1] == ' ')
			{
				return method_value[i];
			}
		}
	}
	return NONE;
}

void	HttpExchange::_initRequest(e_http_method method)
{
	switch (method)
	{
		case HEAD:
			_request = ::new HttpRequestHead(_buffer_read);
			break;
		case GET:
			_request = ::new HttpRequestGet(_buffer_read);
			break;
		case POST:
			_request = ::new HttpRequestPost(_buffer_read);
			break;
		case DELETE:
			_request = ::new HttpRequestDelete(_buffer_read);
			break;
		default:
			protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
				std::string("Error: Http method not implemented in request from")));
			throw ExceptionHttpStatusCode(HTTP_501);
	}
}

void	HttpExchange::_handleError(int fd, Cluster &cluster, e_status_code error)
{
	if (_request != NULL)
		_response.generateErrorResponse(error, _socket->getServer(), _request->getMethod());
	else
		_response.generateErrorResponse(error, _socket->getServer(), g_http_versions.end());
	cluster.switchHttpExchangeToWrite(fd);
}

void	HttpExchange::readSocket(int fd, Cluster &cluster)
{
	try
	{
		bool end = false;
		if (_request != NULL && _request->hasBody() == true)
		{
			_request->readBody(fd, _socket, end);
			if (end == true)
			{
				//////std::cout << "switch write" << std::endl;
				_request->generateResponse(_socket, _response);
				cluster.switchHttpExchangeToWrite(fd);
			}
		}
		else
			_readHeader(fd, cluster);
	}
	catch(ExceptionHttpStatusCode &e)
	{
		e.display_error();
		_handleError(fd, cluster, e.get_status_code());
		return ;
	}
	catch(std::exception &e)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
				std::string("Error: ") + std::strerror(errno) + " at"));
		_handleError(fd, cluster, HTTP_500);
		return ;
	}
}

void	HttpExchange::_handleHeader(int fd, Cluster &cluster)
{
	_initRequest(_findMethod(_buffer_read));
	////_request->displayRequest();
	_setRightSocket(cluster);
	_buffer_read.clear();
	_request->processHeader(_socket);
	if (_request->hasBody() == false)
	{
		////std::cout << "switch write no body" << std::endl;
		_request->generateResponse(_socket, _response);
		cluster.switchHttpExchangeToWrite(fd);
	}
	if (_request->hasCgi())
		cluster.addCgi(_request->getCgi(), this);
}

void	HttpExchange::_readHeader(int fd, Cluster &cluster)
{
	char buffer[READ_SIZE + 1] = {0};

	int ret = read(fd, buffer, READ_SIZE);
	if (ret == -1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: read(): ") + std::strerror(errno) + " at"));
		throw ExceptionHttpStatusCode(HTTP_500);
	}
	if (ret == 0)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: Missing empty line at the end of the http request from")));
		throw ExceptionHttpStatusCode(HTTP_400);
	}
	std::string tmp(buffer, ret);
	_buffer_read += tmp;
	if (_buffer_read.find("\r\n\r\n") != std::string::npos)
		_handleHeader(fd, cluster);
}

void	HttpExchange::writeSocket(int fd, Cluster &cluster)
{
	if (_response.is_response_ready())
	{
		if (_request == NULL)
			_response.writeResponse(fd, cluster, false);
		else
			_response.writeResponse(fd, cluster, _request->hasCgi());
	}
	//// else
	//// 	std::cout << " not ready" << std::endl;
}

static void	_handle_cgi_error(HttpRequest & request, HttpResponse &response,
				e_status_code error, Server const &server, Cluster &cluster, int fd)
{
	request.setCgi(false);
	request.getCgi()->endRead();
	if (!response.is_response_ready())
		response.generateErrorResponse(error, server, request.getMethod());
	else
		cluster.closeConnection(fd);
}

void	HttpExchange::readCgi(int fd, Cluster & cluster)
{
	try
	{
		std::string tmp;
		int ret = _request->getCgi()->read(tmp);
		if (ret == -1)
			throw_http_err_with_log(HTTP_500, "ERROR: fail to read in cgi");
		if (ret == 0)
		{
			if (!_response.is_response_ready())
				throw_http_err_with_log(HTTP_400, "ERROR: Missing empty line at the end of the cgi");
			_response.setEndOfFile();
		}
		if (_response.is_response_ready() && *_request->getMethod() != "HEAD")
			_response.addBodyContent(tmp);
		else
		{
			_buffer_read += tmp;
			if (_buffer_read.find("\r\n\r\n") != std::string::npos)
			{
				_response.parseCgiHeader(_buffer_read);
				_buffer_read.clear();
				if (*_request->getMethod() == "HEAD")
				{
					_response.removeBody();
				}
			}
		}
	}
	catch(ExceptionHttpStatusCode &e)
	{
		e.display_error();
		_handle_cgi_error(*_request, _response, e.get_status_code(), _socket->getServer(), cluster, fd);
		return;
	}
	catch(std::exception &e)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
				std::string("Error: ") + std::strerror(errno) + " at"));
		_handle_cgi_error(*_request, _response, HTTP_500, _socket->getServer(), cluster, fd);
		return;
	}
}

void	HttpExchange::writeCgi(int fd, Cluster & cluster)
{
	try
	{
		if (fd == -1)
			return ;
		bool end = false;
		dynamic_cast<HttpRequestPost*>(_request)->processBody(end);
		if (end == true)
		{
			////std::cout << "switch write cgi" << std::endl;
			_request->generateResponse(_socket, _response);
			cluster.switchHttpExchangeToWrite(fd);
		}
	}
	catch(ExceptionHttpStatusCode &e)
	{
		e.display_error();
		_request->setCgi(false);
		_request->getCgi()->endWrite();
		_handleError(fd, cluster, e.get_status_code());
		return ;
	}
	catch(std::exception &e)
	{
		_request->setCgi(false);
		_request->getCgi()->endWrite();
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
				std::string("Error: ") + std::strerror(errno) + " at"));
		_handleError(fd, cluster, HTTP_500);
		return ;
	}
}
