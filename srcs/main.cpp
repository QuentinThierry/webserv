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
	std::cout << "Host : " << server.getHost() << std::endl;
	std::cout << "Port : " << server.getPort() << std::endl;
	std::cout << "Server_names : " << std::endl;
	for (unsigned int i = 0; i < server.getServerName().size(); i++) {
		std::cout << "\t" << server.getServerName()[i] << std::endl;
	}
	std::cout << "Error path : " << server.getErrorPagePath() << std::endl;
	std::cout << "Client mbs : " << server.getClientmaxBodySize() << std::endl;

	for (unsigned int i = 0; i < server.getLocations().size(); i++) {
		Location const &loc = server.getLocations()[i];

		std::cout << "Location path : " << loc.getLocationPath() << std::endl;
		std::cout << "\tMethods : " << std::endl;
		for (unsigned int j = 0; j < loc.getMethods().size(); j++) {
			std::cout << "\t\t" << loc.getMethods()[j] << std::endl;
		}
		std::cout << "\tRoot path : " << loc.getRootPath() << std::endl;
		std::cout << "\tHas redirect : " << loc.getHasRedirect() << std::endl;
		std::cout << "\tRedirect path : " << loc.getRedirectPath() << std::endl;
		std::cout << "\tDefault dir path : " << std::endl;
		for (unsigned int j = 0; j < loc.getDefaultDirPath().size(); j++) {
			std::cout << "\t\t" << loc.getDefaultDirPath()[j] << std::endl;
		}
		std::cout << "\tHas autoindex : " << loc.getHasAutoindex() << std::endl;
		std::cout << "\tAccept upload : " << loc.getCanAcceptUpload() << std::endl;
		std::cout << "\tDefault upload dir : " << loc.getDefaultUploadPath() << std::endl;
		std::cout << "\tHas CGI : " << loc.getHasCgi() << std::endl;
		std::cout << "\tPath CGI : " << loc.getCgiPath() << std::endl;
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