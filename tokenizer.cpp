#include "ConfParser.hpp"

void	_skip_white_spaces(std::string &buffer, unsigned int &i)
{
	while (buffer[i] == ' ' || buffer[i] == '\t')
		i++;
}

std::string	_get_token(std::string &buffer, unsigned int &i)
{
	if (buffer[i] == '{')
	{
		i++;
		return "{";
	}
	if (buffer[i] == '}')
	{
		i++;
		return "}";
	}
	if (buffer[i] == ';')
	{
		i++;
		return ";";
	}
	int start_pos = i;
	while (i < buffer.size() && buffer[i] != '{' && buffer[i] != '}' && buffer[i] != ';' && buffer[i] != ' ' && buffer[i] != '\t')
		i++;
	return buffer.substr(start_pos, i - start_pos);
}

void	_fill_line_tokens(std::string &buffer, std::queue<std::string> &tokens)
{
	unsigned int i = 0;
	while (i < buffer.size())
	{
		_skip_white_spaces(buffer, i);
		if (i >= buffer.size())
			break;
		tokens.push(_get_token(buffer, i));
	}
}

void	tokenize_file(std::fstream &s, std::queue<std::string> &tokens)
{
	while (true)
	{
		std::string	buffer;
		try
		{
			if (!wrap_getline_throw(s, buffer))
				break ;
			_fill_line_tokens(buffer, tokens);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			return ;
		}
	}
}

