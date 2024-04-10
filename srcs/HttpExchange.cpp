#include "HttpExchange.class.hpp"

#define READ_SIZE 100
#include <cstdlib>


HttpExchange::HttpExchange(Socket const &socket): _socket(&socket){};

// void HttpExchange::read_header(int fd)
// {
// 	char buffer[READ_SIZE + 1] = {0};

// 	int ret = read(fd, buffer, READ_SIZE);
// 	if (ret == -1)
// 	{
// 		return; //error
// 	}
// 	//if (ret == 0) =>eof
// 	std::string str_buffer(buffer);
// 	_buffer_read = _buffer_read + str_buffer;
// 	if (str_buffer.find("\r\n\r\n") != std::string::npos)
// 	{
// 		std::cout << _buffer_read;
// 		std::exit(1);
// 		//fin du header
// 		//_request = _requset(_buffer_read);
// 		//check the right server
// 		//check the right header size/encoding
// 		//check location for cgi
// 		//read body with client_max_body => write in file OR pass to CGI
// 	}
// }

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
read trailer field
while (trailer field is not empty) {
if (trailer field is allowed to be sent in a trailer) {
	append trailer field to existing header fields
}
read trailer-field
}
Content-Length := length
Remove "chunked" from Transfer-Encoding
Remove Trailer from existing header fields
*/