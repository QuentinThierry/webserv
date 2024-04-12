#include "DefineVariableFields.hpp"

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
	if (server.getHost() == LOCALHOST)
		server.setHost(LOCALHOST_RESOLVE);
	server.setPort(str_to_short(token.substr(colon_pos + 1)));
}

void fill_listen(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (server._getHasListen())
		throw std::exception();
	if (location || arg_counter != 1)
		throw std::exception();
	_parse_listen_argument(token, server);
	server._setHasListen(true);
}

void fill_server_name(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (location)
		throw std::exception();
	server.addServerName(token);
}

void fill_error_page(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location)
		throw std::exception();
	if (arg_counter == 1)
		server._setHasFoundErrorPagePath(false);
	if (server._getHasFoundErrorPagePath() == true)
		throw std::exception();
	if (does_http_code_exist(token) == false)
	{
		server._setHasFoundErrorPagePath(true);
		std::map<t_http_code, std::string>::iterator it = --server.getErrorPagePath().end();
		for (unsigned int i = 0; i < arg_counter - 1; i++)
		{
			if (it->second == "")
				it->second = token;
			it--;
		}
	}
	else
		server.addErrorPagePath(str_to_http_code(token), "");
}

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


void fill_client_max_body_size(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (location || arg_counter != 1)
		throw std::exception();
	_parse_client_max_body_size_argument(token, server);
}

