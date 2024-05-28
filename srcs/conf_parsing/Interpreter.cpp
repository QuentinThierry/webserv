#include "ConfParser.hpp"

void	fill_field_value(std::string &token, Server &server, Location *location, t_token_append_function &token_var_function, unsigned int arg_counter)
{
	if (token_var_function == NULL)
		token_var_function = define_token_var_function(token);
	else
		token_var_function(token, server, location, arg_counter);
}

void	interpret_field_loop(std::string &token, std::queue<std::string> &tokens, Server &server, Location *location)
{
	t_token_append_function	token_identifier = NULL;
	unsigned int arg_counter = 0;

	while (!tokens.empty())
	{
		if (token[0] == '{' || token[0] == '}')
			continue;
		else if (token[0] == ';')
		{
			if (arg_counter == 1)
				ThrowMisc("missing argument for `" + func_ptr_to_str(token_identifier) + "`");
			break;
		}
		else
			fill_field_value(token, server, location, token_identifier, arg_counter);
		arg_counter++;
		token = extract_token(tokens);
	}
}

// TODO: add fields to cgi and complete parsing
void interpret_cgi_field_loop(std::string &token, std::queue<std::string> &tokens, CgiLocation &cgiObj)
{
	unsigned int arg_counter = 0;
	std::string identifier = "";

	while (!tokens.empty())
	{
		if (token[0] == '{' || token[0] == '}')
			continue;
		else if (token[0] == ';')
		{
			if (arg_counter != 2)
				ThrowBadArgumentNumber("cgi_path", 2, false);
			break;
		}
		else
		{
			if (arg_counter == 0) // define identifier
				identifier = token;
			else if (identifier == "cgi_path")
			{
				if (arg_counter != 1)
					ThrowBadArgumentNumber("cgi location", 1, arg_counter > 2);
				cgiObj.setExecPath(token);
			}
			if (identifier != "cgi_path")
				ThrowBadArgument(identifier, "cgi location");
		}
		arg_counter++;
		token = extract_token(tokens);
	}
}

void	interpret_location_loop(std::queue<std::string> &tokens, Server &server)
{
	bool is_cgi_loc = false;
	Location location(server.getDefaultLocation());
	CgiLocation cgi_loc;

	std::string token = extract_token(tokens);
	location.setLocationPath(token);
	if (token[0] == '~') // can be a cgi location
	{
		token = extract_token(tokens);
		if (token[0] != '{') // is for sure a cgi location
			is_cgi_loc = true;
		cgi_loc.setExtension(token);
	}
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
		{
			if (is_cgi_loc && !cgi_loc.is_empty_cgi_location())
				server.addCgiLocation(cgi_loc);
			else if (!is_cgi_loc && !location.isEmptyLocation(server.getDefaultLocation()))
			{
				if (location.getDefaultDirPath().empty())
					location.addDefaultDirPath("index.html");
				server.addLocations(location);
			}
			break;
		}
		else
		{
			if (is_cgi_loc)
				interpret_cgi_field_loop(token, tokens, cgi_loc);
			else
				interpret_field_loop(token, tokens, server, &location);
		}
	}
}

// tokens is a copy
void	interpret_server_fields(Server &server, std::queue<std::string> tokens)
{
	bool	skipping_location = false;
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
		{
			if (skipping_location)
			{
				skipping_location = false;
				continue ;
			}
			break ;
		}
		else if (skipping_location)
			continue ;
		else if (token == "location")
			skipping_location = true;
		else
			interpret_field_loop(token, tokens, server, NULL);
	}
}

// tokens is a reference
void	interpret_location_fields(Server &server, std::queue<std::string> &tokens)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
			break ;
		else if (token == "location")
			interpret_location_loop(tokens, server);
	}
}


Server	interpret_server_loop(std::queue<std::string> &tokens)
{
	Server server;
	Location default_location;
	
	server.addLocations(default_location);

	interpret_server_fields(server, tokens);
	interpret_location_fields(server, tokens);
	if (server.getHost().empty())
		ThrowMisc("missing `listen` field");
	if (server.getDefaultLocation().getDefaultDirPath().empty())
		server.getDefaultLocation().addDefaultDirPath("index.html");
	server.removeDuplicatedLocation();
	return server;
}

void	_check_server_is_unique(std::vector<Server> &servers)
{
	std::vector<std::string> &to_test = servers.back().getServerName();
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator it_to_test;

	for (unsigned int i = 0; i < servers.size() - 1; i++)
	{
		if (!servers[i].is_equal(servers.back()))
			continue;
		it = servers[i].getServerName().begin();
		while (it != servers[i].getServerName().end())
		{
			for (it_to_test = to_test.begin(); it_to_test != to_test.end();)
			{
				if (*it == *it_to_test)
				{
					it_to_test = to_test.erase(it_to_test);
					if (to_test.empty())
						return ;
				}
				else
					it_to_test++;
			}
			it++;
		}
		if (to_test.empty())
			return ;
	}
}


void	interpret_tokens(std::queue<std::string> &tokens, std::vector<Server> &servers)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		servers.push_back(interpret_server_loop(tokens));
		_check_server_is_unique(servers);
	}
}