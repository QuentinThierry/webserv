#include "HttpResponse.class.hpp"

HttpResponse::HttpResponse( HttpResponse const & model)
{
	(void)model;
	//TODO
}
HttpResponse & HttpResponse::operator=(HttpResponse const & model )
{
	(void)model;
	//TODO
	return *this;
}
HttpResponse::~HttpResponse( void )
{
	//TODO
}
HttpResponse::HttpResponse( void )
{
	//TODO
}

void	HttpResponse::generateErrorResponse(e_status_code status)
{
	(void)status;
	// _version = "HTTP/1.1";
	// _status_code = status;
}