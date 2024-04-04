#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <exception>
#include "Server.hpp"

// returns false on eof
static bool	wrap_getline_throw(std::fstream &s, std::string &buffer)
{
	getline(s, buffer);
	if (s.fail())
	{
		std::cout << "Failed to read conf file" << std::endl;
		throw std::exception(); // EXCEPTION failed to read
	}
	if (s.eof())
		return false;
	return true;
}


void	check_server_line_syntax(std::string &line, bool &has_bracket)
{
	int i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	line.substr(i);
	if (line.find("server") != 0)
		throw std::exception(); // wrong conf

	i = line.find_first_of('{');
	if (i != std::string::npos)
		has_bracket = true;
	if (line.find_first_of('{', i + 1) != std::string::npos)
		throw std::exception(); //wrong conf
}


void	enter_server(std::vector<Server> &servers, std::fstream &s, std::string &line)
{
	bool	has_bracket = false;

	check_server_line_syntax(line, has_bracket);

	std::cout << has_bracket << std::endl;
}





int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	std::fstream s;
	std::vector<Server> servers;

	s.open(argv[1], std::ios::in);
	if (!s.is_open())
	{
		std::cout << "Failed to open '" << argv[1] << "'" << std::endl;
		return 0;
	}
	std::string buffer;
	int i = 0;
	while (true)
	{
		try
		{
			if (!wrap_getline_throw(s, buffer))
				break ;
			// if server is in line
			if (buffer.find("server") != std::string::npos)
				enter_server(servers, s, buffer);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
		s.clear();
	}
}