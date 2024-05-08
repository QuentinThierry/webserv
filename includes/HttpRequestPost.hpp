#ifndef HTTPREQUESTPOST_HPP
# define HTTPREQUESTPOST_HPP
# include "HttpRequest.hpp"

class HttpRequestPost : public HttpRequest
{
	public:
		HttpRequestPost (std::string const & str_request) throw (ExceptionHttpStatusCode);
		HttpRequestPost ( HttpRequestPost const & model);
		HttpRequestPost & operator= (HttpRequestPost const & model);

		void				readBody(int fd, Socket const * const socket);
		void				process_header( Socket const * const socket );
		bool				hasBody() const;

	private:
		HttpRequestPost( void );
		~HttpRequestPost( void );
		void			_setBodyReadType(uint64_t maxClientBody);
		uint64_t		_getSizeToReadBody(uint64_t maxClientBody) const;


		uint64_t _content_length;
		uint64_t _read_size;

		bool _chunk_body_flags;
		bool _content_length_flags;
};

#endif