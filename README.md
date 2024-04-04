### [HTTP response](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Standard_response_fields)

### [Server conf](http://nginx.org/en/docs/http/ngx_http_core_module.html)

webserv														-> nginx

- Choisir port et host										-> *listen \<host\>:\<port\>* [doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#listen)
- Setup server_names ou pas									-> *server_name \<name\>*[doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#server_name)
- Pages d'erreur par defaut									-> *error_page \<error\> \<path_error_file\>* [doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#error_page)
- Limiter la taille du body des clients						-> client_max_body_size \<size\> (int M or int K) | default : 1M (0 is no restriction) [doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size)
- Dans les routes :											-> *location \<path\> {...}* [doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#location)
	- List de méthodes HTTP acceptées						-> *limit_except METHOD {...}* [doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except) (need discuss)
	- Redirection HTTP										-> *rewrite or return* [doc](http://nginx.org/en/docs/http/ngx_http_rewrite_module.html#rewrite)
	- Repertoire de start de recherche						-> *root \<path\>;* [doc](http://nginx.org/en/docs/http/ngx_http_core_module.html#root)
	- Listing repertoires									-> *autoindex on|off* [doc](http://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex)
	- Fichier par defaut si rep = repertoire				-> *index file1 file2...* : default = index index.html
	- Executer CGI en fonction de certaines extensions		-> ??
	- Faire fonctionner avec GET ou POST					-> ??
	- Accepter les fichiers televerses et config			-> ??


### git rules
commits rules :
	https://gist.github.com/qoomon/5dfcdf8eec66a051ecd85625518cfd13

branch rules :
	https://medium.com/@abhay.pixolo/naming-conventions-for-git-branches-a-cheatsheet-8549feca2534


### Things to send after conf parsing

- host -> str
- port -> int16
- server_name -> str
- error_page -> str
- client_max_body_size -> uint64
- location : -> class
	- location_path -> str
	- accepted_methods -> vector<str>
	- root_path -> str
	- has_redirect -> bool
	- redirect_path -> str
	- has_autoindex -> bool
	- default_dir_file -> str
	- is_accept_upload -> bool
	- upload_path -> str
	- has_cgi -> bool
	- cgi_path -> str