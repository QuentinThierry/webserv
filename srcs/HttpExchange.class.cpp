#include "HttpExchange.class.hpp"
#include "HttpResponseStatus.hpp"
#include "Cluster.class.hpp"
#include "util.hpp"

#include <cstdlib>


HttpExchange::HttpExchange(Socket const &socket): _socket(&socket), _request(){};

// void HttpExchange::_setRightSocket(Cluster const &cluster)
// {
// 	e_status error;

// 	std::vector<std::string> const &host_name = _request->getFieldValue("Host", error);
// 	if (error == FAILURE || host_name.size() != 1)
// 	{
// 		throw ExceptionHttpStatusCode(HTTP_400); //!send error
// 	}
// 	Socket const *socket = cluster.get_matching_socket(_socket->getFd(), host_name.at(0));
// 	if (socket != NULL)
// 		_socket = socket;
// }

// e_http_method HttpExchange::_findMethod()
// {
// 	std::string method_name[3] = {"GET", "POST", "DELETE"};
// 	e_http_method method_value[3] = {GET, POST, DELETE};

// 	for (int i = 0; i < 3; i++)
// 	{
// 		for (int j = 0; j < method_name[i].size(); i++)
// 		{
// 			if (method_name[i][j] != _buffer_read[j])
// 				break;
// 			else if (j == method_name[i].size() - 1 && _buffer_read[j + 1] == ' ')
// 			{
// 				return method_value[i];
// 			}
// 		}
// 	}
// 	return NONE;
// }

// void HttpExchange::_initRequest(e_http_method method)
// {
// 	switch (method)
// 	{
// 		case GET:
// 			_request = ::new HttpRequestPost(_buffer_read); //!to change
// 		case POST:
// 			_request = ::new HttpRequestPost(_buffer_read);
// 		case DELETE:
// 			_request = ::new HttpRequestPost(_buffer_read); //!to change
// 		default:
// 			throw ExceptionHttpStatusCode(HTTP_501);
// 	}
// }

// void HttpExchange::readSocket(int fd, Cluster &cluster)
// {
// 	if (_request.hasBody() == true)
// 		_handleBody(fd, cluster);
// 	else
// 		_handleHeader(fd, cluster);
// }

// void HttpExchange::_handleHeader(int fd, Cluster &cluster)
// {
// 	char buffer[READ_SIZE + 1] = {0};

// 	int ret = read(fd, buffer, READ_SIZE);
// 	if (ret == -1)
// 	{
// 		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
// 					std::string("Error: read() ") + std::strerror(errno)));
// 		return; //!send error to client
// 	}
// 	std::cout << buffer << std::endl;
// 	_buffer_read += buffer;
// 	if (_buffer_read.find("\r\n\r\n") != std::string::npos)
// 	{
// 		std::cout << _buffer_read;
// 		try
// 		{
// 			_initRequest(_findMethod());
// 			_setRightSocket(cluster);
// 			_buffer_read.clear();
			
// 			if (_request.hasBody() == false)
// 				cluster.switchHttpExchangeToWrite(fd);
// 		}
// 		catch(std::exception &e)
// 		{
// 			std::cout << "Error \n";
// 			//!send response();
// 			return ;
// 		}
// 	}
// }

// void HttpExchange::_handleBody(int fd, Cluster &cluster)
// {
// 	char buffer[READ_SIZE + 1] = {0};
// 	int read_size = _request.getSizeToReadBody(_socket->getServer().getClientmaxBodySize());
// 	int ret = read(fd, buffer, read_size);
// 	if (ret == -1)
// 	{
// 		protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
// 					std::string("Error: read() ") + std::strerror(errno)));
// 		return; //!send error to client
// 	}
// 	else if (ret == 0) //EOF
// 	{
// 		cluster.switchHttpExchangeToWrite(fd);
// 	}
// 	else
// 	{
// 		_request.addStringToBody(buffer);
		
// 	}
// }

// void HttpExchange::writeSocket(int fd, Cluster &cluster)
// {
// 	if (_buffer_read.size() != 0)
// 	{
// 		int ret = write(fd, _buffer_read.c_str(), _buffer_read.size());
// 		if (ret == -1 || ret == 0)
// 		{
// 			protected_write(g_err_log_fd, error_message_server(_socket->getServer(),
// 					std::string("Error: write() ") + std::strerror(errno)));
// 			return; //!error
// 		}
// 		else
// 			_buffer_read.clear();
// 	}
// 	(void)cluster;
// 	cluster.closeConnection(fd);
// }


// /*
// READ BODY

// body but no content lenght => 400
// POST has body
// GET no body => ignore body
// DELETE may have a body

// Transfer-Encoding => need to dechunk => have body
// chunking only one time
// Content-Lenght => have body
// A server that receives a request message with a transfer coding it
//    does not understand SHOULD respond with 501 (Not Implemented)

// !Transfer-Encoding overrides the Content-Length

// !Multiple Content-Length header fields having differing field-values or a single Content-Length header field having an invalid value => 400 error

// !If a valid Content-Length header field is present without
// !       Transfer-Encoding, its decimal value defines the expected message
// !       body length in octets.  If the sender closes the connection or
// !       the recipient times out before the indicated number of octets are
// !       received, the recipient MUST consider the message to be
// !       incomplete and close the connection

// no content-legnth + no transfer-Encoding => body size 0 

// // A server MAY reject a request that contains a message body but not a
// //    Content-Length by responding with 411 (Length Required).

// A message body that uses the chunked transfer coding is incomplete if
// 	the zero-sized chunk that terminates the encoding has not been
// 	received

//  A message that uses a valid Content-Length is incomplete
//    if the size of the message body received (in octets) is less than the
//    value given by Content-Length

// DECODING CHUNK
// length := 0
// read chunk-size, chunk-ext (if any), and CRLF
// while (chunk-size > 0) {
// read chunk-data and CRLF
// append chunk-data to decoded-body
// length := length + chunk-size
// read chunk-size, chunk-ext (if any), and CRLF
// }
// Content-Length := length
// Remove "chunked" from Transfer-Encoding
// Remove Trailer from existing header fields
// */
