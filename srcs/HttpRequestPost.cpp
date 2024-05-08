#include "HttpExchange.class.hpp"
#include "HttpRequestPost.hpp"
#include "util.hpp"

bool HttpRequestPost::hasBody() const
{
	return (_chunk_body_flags == true || _content_length_flags == true);
}

void HttpRequestPost::readBody(int fd, Socket const * const socket)
{
	char buffer[READ_SIZE + 1] = {0};
	int read_size = _getSizeToReadBody(socket->getServer().getClientmaxBodySize());
	int ret = read(fd, buffer, read_size);
	if (ret == -1)
	{
		protected_write(g_err_log_fd, error_message_server(socket->getServer(),
					std::string("Error: read() ") + std::strerror(errno)));
		return; //!send error to client
	}
	else if (ret == 0) //EOF
	{
		cluster.switchHttpExchangeToWrite(fd);
	}
	else
	{
		_request.addStringToBody(buffer);
	}
}

uint64_t HttpRequestPost::_getSizeToReadBody(uint64_t max_boby_client) const
{
	if (_content_length_flags == true && _read_size + READ_SIZE > _content_length)
		return _content_length - _read_size;
	else if (_read_size + READ_SIZE > max_boby_client)
		return max_boby_client - _read_size;
	else
		return READ_SIZE;
}

void	HttpRequestPost::process_header(Socket const * const socket)
{
	//check location for cgi
	//read body with client_max_body => write in file
	//check method
	//check location file to upload file
	_setBodyReadType(socket->getServer().getClientmaxBodySize());
	_read_size = _body.size();
	//open file to save data
}

void HttpRequestPost::_setBodyReadType(uint64_t maxClientBody)
{
	_chunk_body_flags = false;
	_content_length_flags = false;
	if (checkFieldExistence("Transfer-Encoding") == true)
	{
		std::vector<std::string> const &encoding = getFieldValue("Transfer-Encoding");
		if (encoding.size() != 1 || encoding[0] != "chunked")
			throw ExceptionHttpStatusCode(HTTP_501); //!send response
		_chunk_body_flags = true;
		return ;
	}
	if (checkFieldExistence("Transfer-Encoding") == true)
	{
		std::vector<std::string> const &content_length = getFieldValue("Content-Length");
		if (content_length.size() != 1)
			throw ExceptionHttpStatusCode(HTTP_400); //!send response
		e_status error = SUCCESS;
		_content_length = ft_atoi(content_length[0], error);
		if (error == FAILURE)
			throw ExceptionHttpStatusCode(HTTP_400); //!send response
		if (_content_length > maxClientBody)
			throw ExceptionHttpStatusCode(HTTP_413); //!send response
		_content_length_flags = true;
		return ;
	}
}