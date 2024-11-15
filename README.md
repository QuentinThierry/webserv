<h1 align=center>💻 Webserv</h1>
<p align="center">
  <img src="img/webserv.png?raw=true" alt="Webserv Project Image"/>
</p>

## About
>This project is about writing your own HTTP server.
>You will be able to test it with an actual browser.
>HTTP is one of the most used protocols on the internet.
>Knowing its arcane will be useful, even if you won’t be working on a website.

[Subject Webserv Project 42](webserv.pdf)

The program runs a HTTP server capable of receiving requests and sending responses.

## Features
- **Description**: A simplified HTTP server inspired by Nginx, capable of handling basic HTTP requests.
- **Feature**: 
	- Supports HTTP methods: GET, POST, DELETE, and HEAD, as well as CGI (Common Gateway Interface).
	- Modular server configuration for flexibility.
- **How to Use**: Run the program `./webserv` with a configuration file as an argument. Access the server via `http://localhost:<port>/<request>`

## Setup

**Clone the Repository**:

```bash
git clone https://github.com/QuentinThierry/webserv.git;
cd webserv
```

## Usage
1. Optionally, create a configuration file with a `.conf` extension.
2. Run the program `webserv` with the configuration file as an argument.
	```bash
	./webserv server.conf
	#or
	./webserv
	```
3. Send HTTP requests to the server using `POST`, `GET`, `HEAD` or `DELETE` methods
	```
	POST /<path> HTTP/1.1
	Host: localhost:<port>
	User-Agent: insomnia/2022.7.0
	Accept: */*
	content-type: application/json
	Content-Length: 48

	{
		"int": "42",
		"float": "42.0",
		"char": "p"
	}
	```
- CGI is supported for the `GET` and `POST` methods.

## Configuration file
The configuration file defines the server's behavior using a modular syntax.
### Key components:
- Define a Server Scope

		Server { ... }

- Define a Location

		Location <path> { ... }

- Define a Cgi Location

		Location ~ .<extension> { ... }

- Define a Field Variables

		<field identifier> <args> ... ;

### Field identifiers :
#### Server only :
- listen [ \<host IP\> | localhost ][ :PORT ]<span style="color:gray"> -> Define the IP and PORT of the current server scope.</span>
- server_name \<server_name\> ...<span style="color:gray"> -> Specify one or more server names to match.</span>
- error_page \<code\> ... \<error_path_path\><span style="color:gray"> -> Define error pages for specific status codes.</span>
- client_max_body_size \<value\>[ K | M ]<span style="color:gray"> -> Set the maximum body size for client requests.</span>
#### Location / Server :
- root \<path\><span style="color:gray"> -> Define the root directory for requests handled by the server or location.</span>
- autoindex \<on | off\><span style="color:gray"> -> Toggle wether directory contents should be listed or not.</span>
#### Location only :
- limit_except \<GET | POST | DELETE\> ...<span style="color:gray"> -> Remove one or more methods in the current location by listing them.</span>
- return \<error_code\> \<new_uri\><span style="color:gray"> -> Redirect a location on a new given uri with an error code.</span><span style="color:#666666"> - this field will discard all other fields</span>
- index \<file_name\><span style="color:gray"> -> If a directory is given in uri, redirect to the first found file from left to right.</span>
- can_upload \<on | off\><span style="color:gray"> -> Toggle if the current location can or can not accept file upload.</span>
#### Cgi Location :
- cgi_path \<absolute_cgi_path\><span style="color:gray"> -> Define the path of the binary that will execute the given file in uri.</span>
- path_info \<absolute_cgi_root\><span style="color:gray"> -> Define where the uri file name should be matched.</span>

### Example:
`server.conf`
```conf
server{
	listen 8088;
	root ./;
	error_page 404 ./server1/404Error.jpg;
	error_page 403 ./server1/403Error.png;
	client_max_body_size 25;
	autoindex on;

	location /
	{
		limit_except POST DELETE;
		root ./server1;
	}
	location /hey
	{
		return 302 http://www.example.com;
	}
	location /Index
	{
		return 302 http://www.google.com;
		index Welcome.html;
		autoindex on;
	}
}
```

## New Notion

### HTTP Protocol

- HTTP: Hyper Text Transfer Protocol.
- Stateless: Each request is independent of others.
#### Request Methods
- `GET`: Retrieve data from the server.
- `HEAD`: Retrieve headers without the body.
- `POST`: Submit data to the server.
- `PUT`: Update data on the server.
- `DELETE`: Remove data from the server.

#### Header
- Headers are separated from the body by an empty line.
- Common headers
	- `Host`: Specifies the IP or URL of the host (mandatory).
	- `User-Agent`: Indicates the client's browser and OS version (mandatory).
	- `Accept-Language`: Specifies the client's preferred language.
	- `Accept-Encoding`: Lists compression formats supported by the client.
	- `Connection`: Defines connection behavior, such as keep-alive.

#### Status Code
- `1xx`: Informational (processing).
- `2xx`: Success.
- `3xx`: Redirect.
- `4xx`: Client Errors.
- `5xx`: Server Errors.

### Socket
- **Bidirectional Communication**: Allows data exchange in both directions.
- **Port Numbers**: Range from 0 to 65,535. Ports 0–1,023 are reserved.

#### Server Workflow
- `socket()`
	- Creates a socket.
	- Components: stream sockets + address domain (e.g., AF_INET) + protocol (e.g., IPPROTO_TCP for TCP).

- `bind()`
	- Associates the socket with a specific IP and port.
- `listen()`
	- Prepares the socket to accept incoming connections.
- `accept()`
	- Accepts a connection request.
- `send/receive`
	- Exchanges data.
