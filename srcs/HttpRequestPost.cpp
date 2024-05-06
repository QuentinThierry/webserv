#include "HttpExchange.class.hpp"
#include "HttpRequestPostPost.hpp"
#include "util.hpp"

bool HttpRequestPost::hasBody() const
{
	return (_chunk_body_flags == true || _content_length_flags == true);
}

uint64_t HttpRequestPost::getContentLength() const
{
	return _content_length;
}

uint64_t HttpRequestPost::getReadSize() const
{
	return _read_size;
}

void HttpRequestPost::addStringToBody( std::string const & extra_body_content)
{
	_read_size += extra_body_content.size();
	_body += extra_body_content;
}

bool HttpRequestPost::getContentLengthFlags() const
{
	return _content_length_flags;
}

bool HttpRequestPost::getChunkBodyFlags() const
{
	return _chunk_body_flags;
}

uint64_t HttpRequestPost::getSizeToReadBody(uint64_t max_boby_client) const
{
	if (_content_length_flags == true && _read_size + READ_SIZE > _content_length)
		return _content_length - _read_size;
	else if (_read_size + READ_SIZE > max_boby_client)
		return max_boby_client - _read_size;
	else
		return READ_SIZE;
}

void	HttpRequestPost::process_header_post(uint64_t max_boby_client)
{
	//check location for cgi
	//read body with client_max_body => write in file
	//check method
	//check location file to upload file
	setBodyReadType(max_boby_client);
	_read_size = _body.size();
	//open file to save data
}

void HttpRequestPost::setBodyReadType(uint64_t max_boby_client)
{
	e_status error = SUCCESS;

	_chunk_body_flags = false;
	_content_length_flags = false;
	std::vector<std::string> const &encoding = getFieldValue("Transfer-Encoding", error);
	if (error != FAILURE)
	{
		if (encoding.size() != 1 || encoding[0] != "chunked")
			throw ExceptionHttpStatusCode(HTTP_501); //!send response
		_chunk_body_flags = true;
		return ;
	}
	error = SUCCESS;
	std::vector<std::string> const &content_length = getFieldValue("Content-Length", error);
	if (error != FAILURE)
	{
		if (content_length.size() != 1)
			throw ExceptionHttpStatusCode(HTTP_400); //!send response
		_content_length = ft_atoi(content_length[0], error);
		if (error == FAILURE)
			throw ExceptionHttpStatusCode(HTTP_400); //!send response
		if (_content_length > max_boby_client)
			throw ExceptionHttpStatusCode(HTTP_413); //!send response
		_content_length_flags = true;
		return ;
	}
}