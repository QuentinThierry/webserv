#include "ConfParser.hpp"

static void	_enter_field_loop(std::queue<std::string> &tokens)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == '}')
			throw std::exception(); //bad
		else if (token[0] == ';')
			break ; // good
	}
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
			throw std::exception(); //bad
		// good
		if (token[0] == '{')
		{
			if (has_second_field == false)
				is_cgi_loc = false;
			if (has_first_field == false)
				throw std::exception();
			break ;
		}
		else
		{
			if (!is_cgi_loc && has_first_field)
				throw std::exception();
			has_first_field = true;
			if (token.size() == 1 && token[0] == '~')
				is_cgi_loc = true;
			else if (is_cgi_loc && !has_second_field)
			{
				if (token[0] != '.')
					throw std::exception();
				has_second_field = true;
			}
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
				throw std::exception(); //bad
			nb_open_bracket++;
			// good
		}
		else if (token[0] == '}')
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
			break ;
		}
		else if (token[0] == ';')
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
		}
		else
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
			_enter_field_loop(tokens);
		}
	}
}

static void	_enter_server_loop(std::queue<std::string> &tokens)
{
	unsigned int nb_open_bracket = 0;

	if (tokens.empty())
		throw std::exception(); //bad
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{')
		{
			if (nb_open_bracket != 0)
				throw std::exception(); //bad
			nb_open_bracket++;
			// good
		}
		else if (token[0] == '}')
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
			break ;
			// good
		}
		else if (token[0] == ';')
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
			// good
		}
		else if (token == "location")
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
			_enter_location_loop(tokens);
		}
		else
		{
			if (nb_open_bracket == 0)
				throw std::exception(); //bad
			_enter_field_loop(tokens);
		}
	}
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
		throw std::exception(); //bad
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token == "server")
			_enter_server_loop(tokens);
		else
			throw std::exception(); //bad // error syntax
	}
}

