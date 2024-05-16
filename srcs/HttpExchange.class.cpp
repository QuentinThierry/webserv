#include "HttpExchange.class.hpp"
#include "HttpResponseStatus.hpp"
#include "HttpRequestPost.class.hpp"
#include "HttpRequestGet.class.hpp"
#include "HttpRequestDelete.class.hpp"

#include "Cluster.class.hpp"
#include "util.hpp"

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

void HttpExchange::_copyRequest(e_http_method method, HttpRequest const * request)
{
	switch (method)
	{
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

struct timeval const & HttpExchange::getAcceptRequestTime() const
{
	return _accept_request_time;
}

void HttpExchange::_setRightSocket(Cluster const &cluster)
{
	e_status error;

	if (_request->checkFieldExistence("Host") == false)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: Missing 'Host' variable in the http request from")));
		throw ExceptionHttpStatusCode(HTTP_400); //!send error
	}
	std::vector<std::string> const &host_name = _request->getFieldValue("Host");
	if (host_name.size() != 1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: Invalid format of 'Host' variable in the http request from")));
		throw ExceptionHttpStatusCode(HTTP_400); //!send error
	}
	Socket const *socket = cluster.get_matching_socket(_socket->getFd(), host_name.at(0));
	if (socket != NULL)
		_socket = socket;
}

e_http_method HttpExchange::_findMethod(std::string const & cmp)
{
	std::string method_name[3] = {"GET", "POST", "DELETE"};
	e_http_method method_value[3] = {GET, POST, DELETE};

	for (unsigned int i = 0; i < 3; i++)
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

void HttpExchange::_initRequest(e_http_method method)
{
	switch (method)
	{
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

void HttpExchange::_handleError(int fd, Cluster &cluster, e_status_code error)
{
	_response.generateErrorResponse(error, _socket->getServer());
	cluster.switchHttpExchangeToWrite(fd);
}


void HttpExchange::readSocket(int fd, Cluster &cluster)
{
	if (_request != NULL && _request->hasBody() == true)
		_request->readBody(fd, _socket);
	else
		_handleHeader(fd, cluster);
}

void HttpExchange::_handleHeader(int fd, Cluster &cluster)
{
	char buffer[READ_SIZE + 1] = {0};

	int ret = read(fd, buffer, READ_SIZE);
	if (ret == -1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: read(): ") + std::strerror(errno) + " at"));
		_handleError(fd, cluster, HTTP_500); //!send error to client
		return;
	}
	if (ret == 0)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: Missing empty line at the end of the http request from")));
		_handleError(fd, cluster, HTTP_400); //!send error to client
		return;
	}
	std::cout << buffer << std::endl;
	_buffer_read += buffer;
	if (_buffer_read.find("\r\n\r\n") != std::string::npos)
	{
		std::cout << _buffer_read;
		try
		{
			_initRequest(_findMethod(_buffer_read));
			_setRightSocket(cluster);
			_buffer_read.clear();
			_request->process_header(_socket);
			if (_request->hasBody() == false)
			{
				_request->generate_response(_socket, _response);
				cluster.switchHttpExchangeToWrite(fd);
			}
		}
		catch(ExceptionHttpStatusCode &e)
		{
			e.display_error();
			_handleError(fd, cluster, e.get_status_code()); //!send error to client
			return ;
		}
		catch(std::exception &e)
		{
			protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: ") + std::strerror(errno) + " at"));
			_handleError(fd, cluster, HTTP_500); //!send error to client
			return ;
		}
	}
}

void HttpExchange::writeSocket(int fd, Cluster &cluster)
{
	_response.writeResponse(fd, cluster);
}


/*
READ BODY

body but no content lenght => 400
POST has body
GET no body => ignore body
DELETE may have a body

Transfer-Encoding => need to dechunk => have body
chunking only one time
Content-Lenght => have body
A server that receives a request message with a transfer coding it
   does not understand SHOULD respond with 501 (Not Implemented)

!Transfer-Encoding overrides the Content-Length

!Multiple Content-Length header fields having differing field-values or a single Content-Length header field having an invalid value => 400 error

!If a valid Content-Length header field is present without
!       Transfer-Encoding, its decimal value defines the expected message
!       body length in octets.  If the sender closes the connection or
!       the recipient times out before the indicated number of octets are
!       received, the recipient MUST consider the message to be
!       incomplete and close the connection

no content-legnth + no transfer-Encoding => body size 0 

// A server MAY reject a request that contains a message body but not a
//    Content-Length by responding with 411 (Length Required).

A message body that uses the chunked transfer coding is incomplete if
	the zero-sized chunk that terminates the encoding has not been
	received

 A message that uses a valid Content-Length is incomplete
   if the size of the message body received (in octets) is less than the
   value given by Content-Length

DECODING CHUNK
length := 0
read chunk-size, chunk-ext (if any), and CRLF
while (chunk-size > 0) {
read chunk-data and CRLF
append chunk-data to decoded-body
length := length + chunk-size
read chunk-size, chunk-ext (if any), and CRLF
}
Content-Length := length
Remove "chunked" from Transfer-Encoding
Remove Trailer from existing header fields
*/
