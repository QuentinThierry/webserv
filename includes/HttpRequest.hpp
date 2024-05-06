#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include "shared.hpp"
# include "HttpResponseStatus.hpp"
# include <vector>

#define READ_SIZE 20

typedef enum{
	GET,
	POST,
	DELETE,
	NONE
} e_http_method;

class HttpRequestLine
{
		HttpRequestLine( std::string const & request_header ) throw (ExceptionHttpStatusCode);
		HttpRequestLine( HttpRequestLine const & model);
		HttpRequestLine & operator=(HttpRequestLine const & model );

		e_http_method const & getMethod( void ) const;
		std::string const & getTarget( void ) const;
		std::string const & getVersion( void ) const;


	private:
		HttpRequestLine( void ){};


		e_http_method	_method;
		std::string		_target;
		std::string		_version;

};

class HttpRequest : public HttpRequestLine
{
    public:
        HttpRequest ( std::string const & str_request ) throw (ExceptionHttpStatusCode);
        HttpRequest ( HttpRequest const & model );
        HttpRequest & operator=(HttpRequest const & model);

        std::vector<std::string> const &getFieldValue(std::string const & filed_name, e_status &success_status) const;

    private:
        HttpRequest ( void );
        virtual ~HttpRequest( void );
        std::vector<std::string> _fields;
        
};

// class HttpRequest : public HttpRequestLine
// {
// 	public:
// 		HttpRequest ( void );
// 		HttpRequest ( std::string const & str_request ) throw (ExceptionHttpStatusCode);
// 		HttpRequest ( HttpRequest const & model );
// 		HttpRequest & operator=(HttpRequest const & model);

// 		void parseRequest(std::string const & str_request) throw (ExceptionHttpStatusCode);

// 		bool hasBody() const; //! added by me
// 		uint64_t getContentLength() const; //! added by me
// 		uint64_t getReadSize() const; //! added by me
// 		void addStringToBody( std::string const & extra_body_content); //! added by me
// 		bool getContentLengthFlags() const;  //! added by me
// 		bool getChunkBodyFlags() const;  //! added by me
// 		void setBodyReadType(uint64_t max_boby_client);		//! added by me
// 		uint64_t getSizeToReadBody(uint64_t max_boby_client) const;

// 		std::vector<std::string> const &getFieldValue(std::string const & filed_name, e_status &success_status) const;

// 		void process_header_post(uint64_t max_boby_client); //! added by me
// 		void process_header_delete(); //! added by me
// 		void process_header_get(); //! added by me
// 		void (HttpRequest::*process_header)(); //! added by me
// 	private:

// 		virtual ~HttpRequest( void );

// 		uint64_t	_read_size;									//! added by me INIT TO 0
// 		uint64_t	_content_length;							//! added by me INIT TO 0

// 		bool	_chunk_body_flags;								//! added by me INIT TO FALSE
// 		bool	_content_length_flags;							//! added by me INIT TO FALSE


// 		std::vector<std::string> _fields;
// 		std::string _body;
// };

#endif