#include "ConfParser.hpp"

// returns false on eof
bool	wrap_getline_throw(std::fstream &s, std::string &buffer)
{
	getline(s, buffer);
	if (s.eof())
	{
		if (buffer.empty())
			return false;
	}
	else if (s.fail())
	{
		std::cout << "Failed to read conf file" << std::endl;
		throw std::exception(); // EXCEPTION failed to read
	}
	return true;
}

void	print_server(Server &server)
{
	Location const &location = server.getLocations()[0];
	
	std::cout << "host " << server.getHost() << std::endl;
	std::cout << "port " << server.getPort() << std::endl;
	// std::cout << "server name " << server.getServerName()[0] << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
	// std::cout << "host " << server.getHost() << std::endl;
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
	try
	{
		std::queue<std::string> tokens;
		tokenize_file(s, tokens);
		s.close();
		parse_tokens(tokens);
		std::vector<Server> servers;
		interpret_tokens(tokens, servers);
		print_server(servers[0]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	
	std::cout << "OK" << std::endl;
}