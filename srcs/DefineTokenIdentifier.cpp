#include "ConfParser.hpp"
#include <cstdlib>
#include <algorithm>

// server only

u_int16_t	str_to_short(std::string str)
{
	for (unsigned int i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i]))
			throw std::out_of_range("non digit");
	}
	if (str.size() > 5)
		throw std::out_of_range("too long port");
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
	colon_pos = token.find_first_of(':', colon_pos + 1);
	if (colon_pos != std::string::npos)
		throw std::out_of_range("parse");
	// host + port
	if (token[0] == ':')
	{
		// only port
		server.setHost("");
		server.setPort(str_to_short(token.substr(1)));
		return ;
	}
	colon_pos = token.find_first_of(':');
	server.setHost(token.substr(0, colon_pos));
	server.setPort(str_to_short(token.substr(colon_pos + 1)));
}

static void _fill_listen(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 1)
		throw std::exception();
	_parse_listen_argument(token, server);
}

static void _fill_server_name(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (location)
		throw std::exception();
	server.addServerName(token);
}

static void _fill_error_page(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	static bool found_path = false;

	if (location)
		throw std::exception();
	if (arg_counter == 1)
		found_path = false;
	if (found_path == true)
		throw std::exception();
	if (does_http_error_code_exist(token) == false)
	{
		found_path = true;
		std::map<uint16_t, std::string>::iterator it = --server.getErrorPagePath().end();
		for (unsigned int i = 0; i < arg_counter - 1; i++)
		{
			if (it->second == "")
				it->second = token;
			it--;
		}
	}
	else
		server.addErrorPagePath(http_error_code_to_uint16(token), "");
}

#define MAX_BITS_CLIENT_MAX_SIZE 9223372036854775807u
#define MAX_BITS_CLIENT_MAX_SIZE_LEN 20
#define MAX_KBITS_CLIENT_MAX_SIZE 9007199254740991u
#define MAX_KBITS_CLIENT_MAX_SIZE_LEN 16
#define MAX_MBITS_CLIENT_MAX_SIZE 8796093022207u
#define MAX_MBITS_CLIENT_MAX_SIZE_LEN 13

bool	_is_overflow_on_unit(std::string &token, char unit)
{
	uint64_t	value;
	char *endptr;

	if (unit == 'b')
	{
		if (token.size() > MAX_BITS_CLIENT_MAX_SIZE_LEN)
			return true;
		value = std::strtoul(token.c_str(), &endptr, 10);
		if (errno == ERANGE || value > MAX_BITS_CLIENT_MAX_SIZE)
			return true;
	}
	else if (unit == 'k')
	{
		if (token.size() > MAX_KBITS_CLIENT_MAX_SIZE_LEN)
			return true;
		value = std::strtoul(token.c_str(), &endptr, 10);
		if (errno == ERANGE || value > MAX_KBITS_CLIENT_MAX_SIZE)
			return true;
	}
	else if (unit == 'm')
	{
		if (token.size() > MAX_MBITS_CLIENT_MAX_SIZE_LEN)
			return true;
		value = std::strtoul(token.c_str(), &endptr, 10);
		if (errno == ERANGE || value > MAX_MBITS_CLIENT_MAX_SIZE)
			return true;
	}
	return false;
}


void	_parse_client_max_body_size_argument(std::string &token, Server &server)
{
	uint64_t	value;
	char		unit;
	size_t		unit_pos;

	unit_pos = token.find_first_of("MmKk");
	if (unit_pos == std::string::npos)
		unit = 'b';
	else
		unit = std::tolower(token[unit_pos]);
	if (unit_pos != std::string::npos)
		token.erase(unit_pos, 1);
	if (token.find_first_not_of("0123456789") != std::string::npos)
		throw std::exception();
	if (_is_overflow_on_unit(token, unit))
		throw std::exception();
	value = std::atol(token.c_str());
	if (unit == 'b')
		server.setClientmaxBodySize(value);
	else if (unit == 'k')
		server.setClientmaxBodySize(value * 1024);
	else if (unit == 'm')
		server.setClientmaxBodySize(value * 1024 * 1024);
}


static void _fill_client_max_body_size(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 1)
		throw std::exception();
	_parse_client_max_body_size_argument(token, server);
}


// location possible

void	_parse_method(std::string &token, Location &loc)
{
	std::vector<std::string>::iterator it;

	if (token != "GET" && token != "DELETE" && token != "POST")
		throw std::exception();
	it = std::find(loc.getMethods().begin(), loc.getMethods().end(), token);
	if (it != loc.getMethods().end())
		loc.getMethods().erase(it);
}

// only in location
static void _fill_limit_except(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (!location)
		throw std::exception();
	_parse_method(token, *location);
}

static void _parse_redirect(std::string &token, Location &loc, unsigned int arg_counter)
{
	if (arg_counter == 1)
		loc.setRedirect(std::make_pair(http_error_code_to_uint16(token), ""));
	else if (arg_counter == 2)
		loc.getRedirect().second = token;
	else
		throw std::exception();
}


static void _fill_redirect(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (!location)
		location = &(server.getDefaultLocation());
	location->setHasRedirect(true);
	_parse_redirect(token, *location, arg_counter);
}

static void _fill_root_path(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (arg_counter != 1)
		throw std::exception();
	if (!location)
		location = &(server.getDefaultLocation());
	location->setRootPath(token);
}


static void _fill_autoindex(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (arg_counter != 1)
		throw std::exception();
	if (!location)
		location = &(server.getDefaultLocation());
	// _parse_autoindex(token, location);
}


static void _fill_default_dir_file(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (!location)
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
	else if (token == "limit_except")
		return _fill_limit_except;
	else if (token == "return")
		return _fill_redirect;
	else if (token == "root")
		return _fill_root_path;
	else if (token == "autoindex")
		return _fill_autoindex;
	else if (token == "index")
		return _fill_default_dir_file;
	else
		throw std::exception();
}