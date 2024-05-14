#include "../includes/HttpRequestPost.class.hpp"
#include "../includes/HttpResponse.class.hpp"

static void	_add_body_from_request_stream( std::string &request_body,
				std::stringstream &stream_request )
{
	if (!stream_request.eof() && stream_request.peek() != EOF
			&& !std::getline(stream_request, request_body, '\0'))
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_SSTREAM_FAIL);
}

HttpRequestPost::HttpRequestPost (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest()
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_SSTREAM_FAIL);
	HttpRequest::init(stream_request);

	if (*getMethod() != "POST")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPPOST_WRONG_METHOD);

	_add_body_from_request_stream(getBody(), stream_request);



	//TODO you can complete the body with socket read read here or using addStringToBody outside of the constructor


}

HttpRequestPost::HttpRequestPost ( HttpRequestPost const & model)
	: HttpRequest(model)
{
}

HttpRequestPost & HttpRequestPost::operator= (HttpRequestPost const & model)
{
	if (&model != this)
		HttpRequest::operator=(model);
	return (*this);
}

HttpRequestPost::HttpRequestPost( void ) //unused
{
}

HttpRequestPost::~HttpRequestPost( void )
{
}

void			HttpRequestPost::process_header( void )
{
	//TODO
}

HttpResponse	HttpRequestPost::generate_response( void )
{
	//TODO
	return (HttpResponse());
}

bool	HttpRequestPost::hasBody() const
{
	// TODO
	    // return (_chunk_body_flags == true || _content_length_flags == true);
	return (false);
}

#if 0

int g_err_log_fd = STDERR_FILENO;

int main ()
{
	std::string request = "POST the_target/ HTTP/1.1\r\nfield1:value1\r\nField2:value1\r\n\r\nThis body is amazing\r\nDon't you think so?";

	try
	{
		HttpRequestPost get_request (request);

		get_request.display_request();
	}
	catch (ExceptionHttpStatusCode & e)
	{
		e.display_error();
	}
	catch (std::exception & e)
	{
		std::cout << "Non http error: " << e.what() << std::endl;
	}
	return (true);
}

#endif