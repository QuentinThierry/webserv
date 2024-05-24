## TODO
- parse nginx.conf =>Quentin
- http request/response =>Alex, Johanne
- socket => Johanne
- CGIs
- Makefile
- html directory => Quentin ?
- cookie and session management ??
- unit tests ? => all

## CODE PROTOTYPE
int main()
{
	parse .conf
	creation socket
	select
	{
		get conf for fd
		event receive header
		{
			while read request + concatène + need to check client size boby
			parse format(guillemet)
			check method protocol and host
			find locations of the server
			{ for each valid location:

				if redirection
					send redir response
				GET/POST/DELETE(request);
				define absolute url with root (host + domain)
				else if prefix
					if directory
						if default file
							send index index.html
						else if autoindex on
							autoindex
						else
							error 4xx
					else if files
						...
				else if extension
					...

			}
		}
	}
}

class
	read
	request
	socket

class GET/POST/DELETE : inherites request

classe methode
	GET/POST/DELETE():check method of location + dechunk body
	absolute_URL
	generate_response() = 0;
	request;

class request: HttpMessage
	class 1 ligne request
		method
		domain
		protocol
	vector <class fields>
		name
		values
	string body

class response: HttpMessage
	class 1 ligne response
		protocol
		status
	vector <class fields>
		name
		values
	string body

class HttpExchange
{
	socket
	<!-- buffer_read -->
	request
	response
}

## CODE Norme

- clean code ??
- functions -> isEmpty() (lowerCamel)
- variables -> variable_name

- class -> Animal (UpperCamel)
- class.method -> getName (lowerCamel)
- private method or attribute + static functions -> _*