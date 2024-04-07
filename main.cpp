#include <iostream>
#include <fstream>
#include <unistd.h>
#include <queue>
#include <exception>

// returns false on eof
static bool	wrap_getline_throw(std::fstream &s, std::string &buffer)
{
	if (s.eof())
		return false;
	getline(s, buffer);
	if (s.fail())
	{
		std::cout << "Failed to read conf file" << std::endl;
		throw std::exception(); // EXCEPTION failed to read
	}
	return true;
}

void	skip_white_spaces(std::string &buffer, unsigned int &i)
{
	while (buffer[i] == ' ' || buffer[i] == '\t')
		i++;
}

std::string	get_token(std::string &buffer, unsigned int &i)
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

void	fill_line_tokens(std::string &buffer, std::queue<std::string> &tokens)
{
	unsigned int i = 0;
	while (i < buffer.size())
	{
		skip_white_spaces(buffer, i);
		if (i >= buffer.size())
			break;
		tokens.push(get_token(buffer, i));
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
			fill_line_tokens(buffer, tokens);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			return ;
		}
	}
}





int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	std::fstream s;

	s.open(argv[1], std::ios::in);
	if (!s.is_open())
	{
		std::cout << "Failed to open '" << argv[1] << "'" << std::endl;
		return 0;
	}
	std::queue<std::string> tokens;
	tokenize_file(s, tokens);

	while (!tokens.empty())
	{
		std::cout << tokens.front() << std::endl;
		tokens.pop();
	}
}