#include "Webconf.hpp"

// returns false on eof
bool	wrap_getline_throw(std::fstream &s, std::string &buffer)
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
	s.close();
	parse_tokens(tokens);

	// while (!tokens.empty())
	// {
	// 	std::cout << tokens.front() << std::endl;
	// 	tokens.pop();
	// }
	std::cout << "All ok :)" << std::endl;
}