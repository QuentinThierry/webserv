#include "HttpResponse.class.hpp"

HttpResponse::HttpResponse( HttpResponse const & model)
{
	*this = model;
}

HttpResponse::HttpResponse( it_version const & version)
{
	_version = version;
	_fields.push_back(HttpField("Connection", "close"));

}


HttpResponse & HttpResponse::operator=(HttpResponse const & model )
{
	if (this != &model)
	{
		_version = model._version;
		_status_code = model._status_code;
		_fields = model._fields;
		_body = model._body;
	}
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

void	HttpResponse::setStatusCode(e_status_code code)
{
	switch (code)
	{
		case HTTP_307:
			_status_code = 307;
		case HTTP_308:
			_status_code = 308;
		case HTTP_400:
			_status_code = 400;
		case HTTP_401:
			_status_code = 401;
		case HTTP_402:
			_status_code = 402;
		case HTTP_403:
			_status_code = 403;
		case HTTP_404:
			_status_code = 404;
		case HTTP_405:
			_status_code = 405;
		case HTTP_406:
			_status_code = 406;
		case HTTP_407:
			_status_code = 407;
		case HTTP_408:
			_status_code = 408;
		case HTTP_409:
			_status_code = 409;
		case HTTP_410:
			_status_code = 410;
		case HTTP_411:
			_status_code = 411;
		case HTTP_412:
			_status_code = 412;
		case HTTP_413:
			_status_code = 413;
		case HTTP_414:
			_status_code = 414;
		case HTTP_415:
			_status_code = 415;
		case HTTP_416:
			_status_code = 416;
		case HTTP_417:
			_status_code = 417;
		case HTTP_418:
			_status_code = 418;
		case HTTP_421:
			_status_code = 421;
		case HTTP_422:
			_status_code = 422;
		case HTTP_423:
			_status_code = 423;
		case HTTP_424:
			_status_code = 424;
		case HTTP_425:
			_status_code = 425;
		case HTTP_426:
			_status_code = 426;
		case HTTP_428:
			_status_code = 428;
		case HTTP_429:
			_status_code = 429;
		case HTTP_431:
			_status_code = 431;
		case HTTP_451:
			_status_code = 451;
		case HTTP_500:
			_status_code = 500;
		case HTTP_501:
			_status_code = 501;
		case HTTP_502:
			_status_code = 502;
		case HTTP_503:
			_status_code = 503;
		case HTTP_504:
			_status_code = 504;
		case HTTP_505:
			_status_code = 505;
		case HTTP_506:
			_status_code = 506;
		case HTTP_507:
			_status_code = 507;
		case HTTP_508:
			_status_code = 508;
		case HTTP_510:
			_status_code = 510;
		case HTTP_511:
			_status_code = 511;
		default:
			throw (ExceptionUnknownStatusCode());
	}
}

bool	HttpResponse::handle_redirect(Location const & location)
{
	if (location.getHasRedirect())
	{
		setStatusCode(location.getRedirect().first);
		_fields.push_back(HttpField("Location", location.getRedirect().second));
		return true;
	}
	return false;
}


void	HttpResponse::generateErrorResponse(e_status_code status)
{
	setStatusCode(status);
}