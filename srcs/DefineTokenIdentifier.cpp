#include "ConfParser.hpp"

// server only

u_int16_t	str_to_short(std::string str)
{
	for (unsigned int i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i]))
			throw std::exception();
	}
	if (str.size() > 5)
		throw std::exception();
	return std::atoi(str.c_str());
}

void	_parse_listen_argument(std::string &token, Server &server)
{
	std::string host = "";
	size_t colon_pos = 0;

	colon_pos = token.find_first_of(':');
	if (colon_pos == std::string::npos)
	{
		// only port
		server.setHost("");
		server.setPort(str_to_short(token));
		return ;
	}
	colon_pos = token.find_first_of(':', colon_pos);
	if (colon_pos != std::string::npos)
		throw std::exception();
	// host + port
	if (token[0] == ':')
	{
		// only port
		server.setHost("");
		server.setPort(str_to_short(token.substr(1)));
		return ;
	}
	unsigned int i = 0;
	colon_pos = token.find_first_of(':');
	server.setHost(token.substr(0, colon_pos));
	server.setPort(str_to_short(token.substr(colon_pos)));
}

static void _fill_listen(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 0)
		throw std::exception();
	_parse_listen_argument(token, server);
}

static void _fill_server_name(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (location)
		throw std::exception();
	server.addServerName(token); // check name syntax ?
}

static void _fill_error_page(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 0)
		throw std::exception();
	server.setErrorPagePath(token);
}

static void _fill_client_max_body_size(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (location)
		throw std::exception();
	// _parse_client_max_body_size_argument(token, server);
}

// location possible
static void _fill_limit_except(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 0)
		location = &(server.getDefaultLocation());
	// _parse_method(token, location);
}

static void _fill_redirect_path(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 0)
		location = &(server.getDefaultLocation());
	location->setRedirectPath(token);
}

static void _fill_root_path(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 0)
		location = &(server.getDefaultLocation());
	location->setRootPath(token);
}


static void _fill_autoindex(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 0)
		location = &(server.getDefaultLocation());
	// _parse_autoindex(token, location);
}


static void _fill_default_dir_file(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location)
		location = &(server.getDefaultLocation());
	location->addDefaultDirPath(token);
}

t_token_append_function define_token_var_function(std::string &token)
{
	// find function that matches token
	if (token == "listen")
		return _fill_listen;
	else if (token == "server_name")
		return _fill_server_name;
	else if (token == "error_page")
		return _fill_error_page;
	else if (token == "client_max_body_size")
		return _fill_client_max_body_size;
	else if (token == "imit_except")
		return _fill_limit_except;
	else if (token == "return")
		return _fill_redirect_path;
	else if (token == "root")
		return _fill_root_path;
	else if (token == "autoindex")
		return _fill_autoindex;
	else if (token == "index")
		return _fill_default_dir_file;
	else
		return NULL;
}