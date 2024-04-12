#include "ConfParser.hpp"

void	fill_field_value(std::string &token, Server &server, Location *location, t_token_append_function &token_var_function, unsigned int arg_counter)
{
	if (token_var_function == NULL)
	{
		token_var_function = define_token_var_function(token);
		if (token_var_function)
			std::exception(); // unknown identifier name
	}
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
				throw std::exception();
			break;
		}
		else
			fill_field_value(token, server, location, token_identifier, arg_counter);
		arg_counter++;
		token = extract_token(tokens);
	}
}

void	interpret_location_loop(std::queue<std::string> &tokens, Server &server)
{
	Location location(server.getDefaultLocation());

	std::string token = extract_token(tokens);
	location.setLocationPath(token);

	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
		{
			server.addLocations(location);
			break;
		}
		else
			interpret_field_loop(token, tokens, server, &location);
	}
}

Server	interpret_server_loop(std::queue<std::string> &tokens)
{
	Server server;
	Location default_location;


	server.addLocations(default_location);
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == '}' || token[0] == ';')
			continue;
		else if (token == "location")
			interpret_location_loop(tokens, server);
		else
			interpret_field_loop(token, tokens, server, NULL);
	}
	return server;
}



void	interpret_tokens(std::queue<std::string> &tokens, std::vector<Server> &servers)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		servers.push_back(interpret_server_loop(tokens));
	}
}