#include "DefineVariableFields.hpp"

void	_parse_method(std::string &token, Location &loc)
{
	std::vector<std::string>::iterator it;

	if (token != "GET" && token != "DELETE" && token != "POST")
		ThrowBadArgument(token, "limit_except");
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
		ThrowBadFieldLocation("location", "limit_except");
	_parse_method(token, *location);
}

static void _parse_redirect(std::string &token, Location &loc, unsigned int arg_counter)
{
	if (arg_counter == 1)
		loc.setRedirect(std::make_pair(str_to_http_code(token), ""));
	else if (arg_counter == 2)
		loc.getRedirect().second = token;
	else
		ThrowBadArgumentNumber("return", 2, true);
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
		ThrowBadArgumentNumber("root", 1, true);
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
		ThrowBadArgument(token, "autoindex");
}

void fill_autoindex(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	if (arg_counter != 1)
		ThrowBadArgumentNumber("autoindex", 1, true);
	if (!location)
		location = &(server.getDefaultLocation());
	_parse_autoindex(token, *location);
}


void fill_default_dir_file(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)arg_counter;
	if (!location)
		location = &(server.getDefaultLocation());
	location->addDefaultIndexPath(token);
}

void fill_can_upload(std::string &token, Server &server, Location *location, unsigned int arg_counter)
{
	(void)server;
	if (!location)
		ThrowBadFieldLocation("server", "can_upload");
	if (arg_counter != 1)
		ThrowBadArgumentNumber("can_upload", 1, true);
	if (token == "on")
		location->setCanUpload(true);
	else if (token == "off")
		location->setCanUpload(false);
	else
		ThrowBadArgument(token, "can_upload");
}
