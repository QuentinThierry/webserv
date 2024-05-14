# Webserv

## New Notion

### HTTP Protocol

- Hyper Text Transfer Protocol
- Stateless => independent request
- Connnectionless

A server MUST NOT send a Transfer-Encoding header field in any
response with a status code of 1xx (Informational) or 204 (No
Content)

#### RFC 7230 7235

#### RFC 4229
   A-IM                      http
   Accept                    http  V
	- Accept:*/*, text/htlm*;q=0.2;level=2
	- no Accept => */*
	- no respond according to Accept => 406 error
	- more specific requst win
   Accept-Additions          http
Accept-Charset            http  V
	- Accept-Charset:UTF;q=0.2
	- * => everything
	- NOT * => nothing except ISO-8859-1
	- NOT Accept-Charset => everything
	- Accept-Charset no response => 406 error
   Accept-Encoding           http  V
	- Accept-Encoding: compress;q=0.5, gzip;q=1.0
	- "identify" always acceptable, unless *;q=0
	- * =>everything
	- Accept-Encoding no response => 406 error
	- HTTP/1.0 don't recognize q=0.3
   Accept-Features           http
Accept-Language           http  V
	- Accept-Language: da, en-gb;q=0.8, en;q=0.7
   Accept-Ranges             http
	- Accept-Ranges: none => for us
   Age                       http  ?? MUST be use for cache
Allow                     http
	- Allow: GET, HEAD, PUT
	- MUST be present for a 405 error
   Alternates                http
   Authentication-Info       http
Authorization             http  ???
   C-Ext                     http
   C-Man                     http
   C-Opt                     http
   C-PEP                     http
   C-PEP-Info                http
   Cache-Control             http  ???
   Connection                http
	- Connection: close
   Content-Base              http
   Content-Disposition       http
   Content-Encoding          http
	- Content-Encoding: gzip
	- define encoding of the body to decode, next
	- if body is not "identity", then MUST include Content-Encoding
	- request encoding not accptable on the server => 415 Error
	- list encoding apply in the same order
   Content-ID                http
Content-Language          http
	- Content-Language: da, fr, en
	- not limited to text
Content-Length            http
	- Content-Length: 3495
	- length in decimal number of OCTETs >= 0
Content-Location          http  ???
   Content-MD5               http  ???
   Content-Range             http
	- go with Accept-Range
   Content-Script-Type       http
   Content-Style-Type        http
Content-Type              http
	- Content-Type: text/html; charset=ISO-8859-4
   Content-Version           http
   Cookie                    http
   Cookie2                   http
   DAV                       http
Date                      http
	- Date: Tue, 15 Nov 1994 08:12:31 GMT
	- MUST include Date header in all response except:
		- status code 100 or 101 or 500 or 503
		- no clock on the server
   Default-Style             http
   Delta-Base                http
   Depth                     http
   Derived-From              http
   Destination               http
   Differential-ID           http
   Digest                    http
   ETag                      http  ??
   Expect                    http
	- error => dont' understand or unable to comply
	- 417 => problems with request 4xx or exceptation can't be met
	- Many older HTTP/1.0 and HTTP/1.1 applications do not understand the Expect header.
   Expires                   http  ??
   Ext                       http
   From                      http  X
   GetProfile                http
Host                      http
	- obligation in all header request
	- server send 400 error if no host present
   IM                        http
   If                        http
   If-Match                  http
   If-Modified-Since         http
   If-None-Match             http
   If-Range                  http
   If-Unmodified-Since       http
   Keep-Alive                http
   Label                     http
Last-Modified             http
   Link                      http
Location                  http
   Lock-Token                http
   MIME-Version              http
   Man                       http
   Max-Forwards              http
   Meter                     http
   Negotiate                 http
   Opt                       http
   Ordering-Type             http
   Overwrite                 http
   P3P                       http
   PEP                       http
   PICS-Label                http
   Pep-Info                  http
   Position                  http
   Pragma                    http
   ProfileObject             http
   Protocol                  http
   Protocol-Info             http
   Protocol-Query            http
   Protocol-Request          http
   Proxy-Authenticate        http
   Proxy-Authentication-Info http
   Proxy-Authorization       http
   Proxy-Features            http
   Proxy-Instruction         http
   Public                    http
   Range                     http
Referer                   http
Retry-After               http
	- use for 503 error or may use for 3xx response
	- Retry-After: Fri, 31 Dec 1999 23:59:59 GMT
	- Retry-After: 120 => in second
   Safe                      http
   Security-Scheme           http
Server                    http
	- software used by the origin server to handle the request
   Set-Cookie                http
   Set-Cookie2               http
   SetProfile                http
   SoapAction                http
   Status-URI                http
   Surrogate-Capability      http
   Surrogate-Control         http
   TCN                       http
   TE                        http
   Timeout                   http
   Trailer                   http
Transfer-Encoding         http
	- type of transformation has been applied to the message body
	- http/1.0 => don't understand Transfer-Coding
   URI                       http
   Upgrade                   http
User-Agent                http  X
   Variant-Vary              http
   Vary                      http
   Via                       http
WWW-Authenticate          http
	- 401 error must include this flags
   Want-Digest               http
   Warning                   http

#### Request
- GET => fetch data from the server
	- don't use it for a form
- POST => add data to the server
- PUT => edit data already on the server
- DELETE => delete data from the server

#### Header
- ONE EMPTY LINE BEEETWEN HEADER AND CORE
- Host => IP or URL of the host
- User-Agent => navigator + OS version
- Accept-Language => client prefer language
- Accept-Encoding => compression treat by the client
- Connection => keep connection or waiting time


#### Status Code
- 1xx => Informational, it's processing
- 2xx => Success
- 3xx => Redirect
- 4xx => Client Error
- 5xx => Server Error

### Socket
- bidirectionnel communication
- communication if same type and domain of socket
- send() -> envoyer
- port (16 bits 0 à 65 535) -> 0 à 1023 réservé

#### client
- socket() -> create socket
	- socket type : stream sockets  + address domain : AT_INET + protocole : IPPROTO_TCP = TCP
- connect() -> connect to the address of the server
- read()/write()

#### Server
- socket()
- bind() ->Internet => port number on the machine
- listen()
- accept()
- send/receive data
