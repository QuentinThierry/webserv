#include "DefineVariableFields.hpp"

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
void fill_limit_except(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)server;
	(void)arg_counter;
	if (!location)
		throw std::exception();
	_parse_method(token, *location);
}

static void _parse_redirect(std::string &token, Location &loc, unsigned int arg_counter)
{
	if (arg_counter == 1)
		loc.setRedirect(std::make_pair(str_to_http_code(token), ""));
	else if (arg_counter == 2)
		loc.getRedirect().second = token;
	else
		throw std::exception();
}


void fill_redirect(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (!location)
		location = &(server.getDefaultLocation());
	location->setHasRedirect(true);
	_parse_redirect(token, *location, arg_counter);
}

void fill_root_path(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (arg_counter != 1)
		throw std::exception();
	if (!location)
		location = &(server.getDefaultLocation());
	location->setRootPath(token);
}

void	_parse_autoindex(std::string &token, Location &loc)
{
	if (token == "on")
		loc.setHasAutoindex(true);
	else if (token == "off")
		loc.setHasAutoindex(false);
	else
		throw std::exception();
}

void fill_autoindex(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (arg_counter != 1)
		throw std::exception();
	if (!location)
		location = &(server.getDefaultLocation());
	_parse_autoindex(token, *location);
}


void fill_default_dir_file(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (!location)
		location = &(server.getDefaultLocation());
	location->addDefaultDirPath(token);
}

void fill_upload_path(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)server;
	if (arg_counter != 1 || !location)
		throw std::exception();
	location->setUploadPath(token);
}