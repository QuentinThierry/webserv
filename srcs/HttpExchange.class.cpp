#include "HttpExchange.class.hpp"
#include "HttpResponseStatus.hpp"
#include "Cluster.class.hpp"

#define READ_SIZE 20
#include <cstdlib>
#include "error.hpp"


HttpExchange::HttpExchange(Socket const &socket): _socket(&socket),
		_size_read(0), _size_chunk(0) ,_method(NONE){};

void HttpExchange::_setRightSocket(Cluster const &cluster)
{
	e_status error;

	std::vector<std::string> const &host_name = _request.getFieldValue("Host", error);
	if (error == FAILURE || host_name.size() != 1)
	{
		throw ExceptionHttpStatusCode(HTTP_400);
		std::exit(0);
	}
	Socket const *socket = cluster.get_matching_socket(_socket->getFd(), host_name.at(0));
	if (socket != NULL)
		_socket = socket;
}

void HttpExchange::_setMethod()
{
	std::string method_name[3] = {"GET", "POST", "DELETE"};
	e_http_method method_value[3] = {GET, POST, DELETE};

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < method_name[i].size(); i++)
		{
			if (method_name[i][j] != _buffer_read[j])
				break;
			else if (j == method_name[i].size() - 1)
			{
				_method = method_value[i];
				return ;
			}
		}
	}
	_method = NONE;
}

void HttpExchange::_init_request()
{
	switch (_method)
	{
		case GET:
			_request.method_get(_buffer_read);
		case POST:
			_request.method_post(_buffer_read);
		case DELETE:
			_request.method_delete(_buffer_read);
		default:
			throw ExceptionHttpStatusCode(HTTP_501);
	}
}



void HttpExchange::readSocket(int fd, Cluster &cluster)
{
	if (_method == POST)
		_handle_body(fd, cluster);
	else
		_handle_header(fd, cluster);
}

void HttpExchange::_handle_header(int fd, Cluster &cluster)
{
	char buffer[READ_SIZE + 1] = {0};

	int ret = read(fd, buffer, READ_SIZE);
	if (ret == -1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: read() ") + std::strerror(errno)));
		return; //!send error to client
	}
	std::cout << buffer << std::endl;
	std::string str_buffer(buffer);
	_buffer_read += str_buffer;
	if (str_buffer.find("\r\n\r\n") != std::string::npos)
	{
		
		//fin du header
		std::cout << _buffer_read;
		// std::exit(1);
		_setMethod();
		try{
			_init_request();
			_buffer_read.clear();
			//check the right server
			_setRightSocket(cluster);
		}
		catch(std::exception &e)
		{
			std::cout << "Error \n";
			//send response();
			return ;
		}
		cluster.switchHttpExchangeToWrite(fd);
		//check the right header size/encoding
		//if client max body size < content lenght => send error
		//check location for cgi
		//read body with client_max_body => write in file OR pass to CGI
	}
}

void HttpExchange::_handle_body(int fd, Cluster &cluster)
{
	char buffer[READ_SIZE + 1] = {0};
	int read_size = READ_SIZE;

	if (_size_read + READ_SIZE > _socket->getServer().getClientmaxBodySize())
		read_size = _socket->getServer().getClientmaxBodySize() - _size_read;
	else if (_size_read + READ_SIZE > _request.method_post.getContentLenght()) // content-lenght
		read_size = _request.method_post.getContentLenght() - _size_read;
	int ret = read(fd, buffer, read_size);
	if (ret == -1)
	{
		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: read() ") + std::strerror(errno)));
		return; //!send error to client
	}
	// if (ret != read_size)

	_buffer_read += buffer;
	_size_read += read_size;
}

void HttpExchange::writeSocket(int fd, Cluster &cluster)
{
	if (_buffer_read.size() != 0)
	{
		int ret = write(fd, _buffer_read.c_str(), _buffer_read.size());
		if (ret == -1 || ret == 0)
		{
			protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
					std::string("Error: write() ") + std::strerror(errno)));
			return; //!error
		}
		else
			_buffer_read.clear();
	}
	(void)cluster;
	cluster.closeConnection(fd);
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

Transfer-Encoding overrides the Content-Length

Multiple Content-Length header fields having differing field-values or a single Content-Length header field having an invalid value => 400 error

If a valid Content-Length header field is present without
       Transfer-Encoding, its decimal value defines the expected message
       body length in octets.  If the sender closes the connection or
       the recipient times out before the indicated number of octets are
       received, the recipient MUST consider the message to be
       incomplete and close the connection

no content-legnth + no transfer-Encoding => body size 0 

A server MAY reject a request that contains a message body but not a
   Content-Length by responding with 411 (Length Required).

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
