#include "ConfParser.hpp"

static void	_enter_field_loop(std::queue<std::string> &tokens)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == '}')
			ThrowUnexpectedToken(token);
		else if (token[0] == ';')
			break ;
	}
}

static bool is_good_extension(std::string extension)
{
	if (extension.size() < 2)
		return false;
	if (extension[0] != '.')
		return false;
	if (extension.find("..") != std::string::npos)
		return false;
	if (extension.find("/") != std::string::npos)
		return false;
	return true;
}

static void	_parse_location_first_part(std::queue<std::string> &tokens)
{
	bool	has_first_field = false;
	bool	has_second_field = false;
	bool	is_cgi_loc = false;

	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '}' || token[0] == ';')
			ThrowUnexpectedToken(token);
		if (token[0] == '{')
		{
			if (has_second_field == false)
				is_cgi_loc = false;
			if (has_first_field == false)
				ThrowUnexpectedToken(token);
			break ;
		}
		else
		{
			if (!is_cgi_loc && has_first_field)
				ThrowUnexpectedToken(token);
			has_first_field = true;
			if (token.size() == 1 && token[0] == '~')
				is_cgi_loc = true;
			else if (is_cgi_loc && !has_second_field)
			{
				if (!is_good_extension(token))
					ThrowMisc("misformatted cgi extension at `" + token + "'");
				has_second_field = true;
			}
			else if (has_first_field && has_second_field)
				ThrowUnexpectedToken(token);
		}
	}
}

static void	_enter_location_loop(std::queue<std::string> &tokens)
{
	_parse_location_first_part(tokens);
	unsigned int nb_open_bracket = 1;

	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{')
		{
			if (nb_open_bracket != 0)
				ThrowUnexpectedToken(token);
			nb_open_bracket++;
		}
		else if (token[0] == '}')
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
			break ;
		}
		else if (token[0] == ';')
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
		}
		else
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
			_enter_field_loop(tokens);
		}
	}
}

static void	_enter_server_loop(std::queue<std::string> &tokens)
{
	unsigned int nb_open_bracket = 0;

	if (tokens.empty())
		ThrowUnexpectedToken("server");
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{')
		{
			if (nb_open_bracket != 0)
				ThrowUnexpectedToken(token);
			nb_open_bracket++;
		}
		else if (token[0] == '}')
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
			nb_open_bracket--;
			break ;
		}
		else if (token[0] == ';')
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
		}
		else if (token == "location")
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
			_enter_location_loop(tokens);
		}
		else
		{
			if (nb_open_bracket == 0)
				ThrowUnexpectedToken(token);
			_enter_field_loop(tokens);
		}
	}
	if (nb_open_bracket != 0)
		ThrowMisc("unexpected EOF");
}

std::string	extract_token(std::queue<std::string> &tokens)
{
	std::string token = tokens.front();
	tokens.pop();
	return token;
}

void	parse_tokens(std::queue<std::string> tokens)
{
	if (tokens.empty())
		ThrowUnexpectedToken("");
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token == "server")
			_enter_server_loop(tokens);
		else
			ThrowUnexpectedToken(token);
	}
}

std::vector<Server>	parse_config(char const *filename)
{
	std::fstream s;

	s.open(filename, std::ios::in);
	if (!s.is_open())
		ThrowMisc("Failed to open '" + std::string(filename) + "'");
	std::queue<std::string> tokens;
	tokenize_file(s, tokens);
	s.close();
	parse_tokens(tokens);
	std::vector<Server> servers;
	interpret_tokens(tokens, servers);
	return servers;
}
