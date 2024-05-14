#include "DefineVariableFields.hpp"

bool	_is_correct_ip(std::string ip)
{
	unsigned int nb_digits = 0;
	unsigned int nb_dots = 0;

	for (unsigned int i = 0; i < ip.size(); i++)
	{
		if ((!std::isdigit(ip[i]) && ip[i] != '.') || nb_dots > 3)
			return false;
		if (ip[i] == '.' && (nb_digits == 0 || nb_digits > 3))
			return false;
		if (ip[i] == '.')
		{
			nb_dots++;
			nb_digits = 0;
		}
		else
			nb_digits++;
	}
	if (nb_digits > 3 || nb_dots != 3)
		return false;
	return true;
}

uint32_t	_convert_host_to_uint(std::string const &host)
{
	std::string tmp = "";
	u_ip_uint32 res = {0};
	unsigned int j = 0;
	int tmp_val = 0;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (; j < host.size(); j++) {
			if (host[j] == '.')
			{
				j++;
				break;
			}
			tmp += host[j];
		}
		if (tmp.size() > 3)
			ThrowMisc("host format denied");
		tmp_val = std::atoi(tmp.c_str());
		if (tmp_val > 255)
			ThrowMisc("host format denied");
		*((uint8_t *)(&res) + i) = tmp_val;
		tmp.clear();
	}
	return ntohl(res.ip);
}

u_int16_t	str_to_short(std::string str)
{
	for (unsigned int i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i]))
			ThrowMisc("host format denied");
	}
	if (str.size() > 5)
		ThrowMisc("host format denied");
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
		server.setHost("0");
		server.setPort(str_to_short(token));
		return ;
	}
	colon_pos = token.find_first_of(':', colon_pos + 1);
	if (colon_pos != std::string::npos)
		ThrowMisc("host format denied");
	// host + port
	if (token[0] == ':')
	{
		// only port
		server.setHost("0");
		server.setPort(str_to_short(token.substr(1)));
		return ;
	}
	colon_pos = token.find_first_of(':');
	server.setHost(token.substr(0, colon_pos));
	if (server.getHost() == LOCALHOST)
		server.setHost(LOCALHOST_RESOLVE);
	else if (!_is_correct_ip(server.getHost()))
		ThrowMisc("host format denied");
	server.setHostUint(_convert_host_to_uint(server.getHost()));
	server.setPort(str_to_short(token.substr(colon_pos + 1)));
}

void fill_listen(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (server._gethasListen() == true)
		ThrowMisc("Too much listen arguments");
	if (location)
		ThrowBadFieldLocation("location", "listen");
	if (arg_counter != 1)
		ThrowBadArgumentNumber("listen", 1, arg_counter > 1);
	_parse_listen_argument(token, server);
	server._sethasListen(true);
}

void fill_server_name(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (location)
		ThrowBadFieldLocation("location", "server_name");
	server.addServerName(token);
}

void fill_error_page(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	static bool has_error_path = false;
	if (location)
		ThrowBadFieldLocation("location", "error_page");
	if (arg_counter == 1)
		has_error_path = false;
	if (has_error_path == true)
		ThrowMisc("error_path acepts only one path");
	if (does_http_code_exist(token) == false)
	{
		has_error_path = true;
		for (std::map<t_http_code, std::string>::iterator it = server.getErrorPagePath().begin(); it != server.getErrorPagePath().end(); it++)
		{
			if (it->second == "") // fill the second part of the map for each error code
				it->second = token;
		}
	}
	else
		server.addErrorPagePath(str_to_http_code(token), ""); // fill the first part of the pair, the error code
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
		ThrowMisc("client_max_body_size value is incorrect");
	if (_is_overflow_on_unit(token, unit))
		ThrowMisc("client_max_body_size value is incorrect");
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
	if (location)
		ThrowBadFieldLocation("location", "client_max_body_size");
	if(arg_counter != 1)
		ThrowBadArgumentNumber("client_max_body_size", 1, arg_counter > 1);
	_parse_client_max_body_size_argument(token, server);
}

