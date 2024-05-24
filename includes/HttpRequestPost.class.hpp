#ifndef HTTPREQUESTPOST_CLASS_HPP
# define HTTPREQUESTPOST_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpRequest.class.hpp"

# define MSG_ERR_HTTPPOST_SSTREAM_FAIL "ERROR: internal: stringstream failure in the post request construction"
# define MSG_ERR_HTTPPOST_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (POST)"

class HttpRequestPost : public HttpRequest
{
	public:
		HttpRequestPost (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestPost ( HttpRequestPost const & model);
		HttpRequestPost & operator= (HttpRequestPost const & model);
		~HttpRequestPost( void );

		void	generateResponse( Socket const * const socket, HttpResponse &response );
		void			processHeader( Socket const * const socket );
		void			readBody(int fd, Socket const * const socket);
		bool			hasBody() const;

	private:
		HttpRequestPost( void );
		void			_setBodyReadType(uint64_t maxClientBody);
		uint64_t		_getSizeToReadBody(uint64_t maxClientBody) const;
		// int		fd;

		uint64_t _content_length;
		uint64_t _read_size;

		bool _chunk_body_flags;
		bool _content_length_flags;
};

#endif
