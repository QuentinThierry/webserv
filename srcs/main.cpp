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

#include <arpa/inet.h>

void	_print_real_host_val(std::string const &host)
{
	struct sockaddr_in addr;

	inet_aton(host.c_str(), &addr.sin_addr);

	std::cout << " (should be " << ntohl(addr.sin_addr.s_addr) << ")" << std::endl;
}

void	print_server(Server &server)
{
	std::cout << "Host : " << server.getHost() << std::endl;
	std::cout << "Host uint : " << server.getHostUint();
	_print_real_host_val(server.getHost());
	std::cout << "Port : " << server.getPort() << std::endl;
	std::cout << "Server_names : " << std::endl;
	for (unsigned int i = 0; i < server.getServerName().size(); i++) {
		std::cout << "\t" << server.getServerName()[i] << std::endl;
	}
	std::cout << "Error path : " << std::endl;
	for (std::map<t_http_code, std::string>::iterator it = server.getErrorPagePath().begin(); it != server.getErrorPagePath().end(); it++) {
		std::cout << "\t" << it->first << " " << it->second << std::endl;
	}
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
		std::cout << "\tRedirect path : " << loc.getRedirect().first << " " << loc.getRedirect().second << std::endl;
		std::cout << "\tDefault dir path : " << std::endl;
		for (unsigned int j = 0; j < loc.getDefaultDirPath().size(); j++) {
			std::cout << "\t\t" << loc.getDefaultDirPath()[j] << std::endl;
		}
		std::cout << "\tHas autoindex : " << loc.getHasAutoindex() << std::endl;
		std::cout << "\tUpload dir : " << loc.getUploadPath() << std::endl;
		std::cout << "\tHas CGI : " << loc.getHasCgi() << std::endl;
		std::cout << "\tPath CGI : " << loc.getCgiPath() << std::endl;
	}
	std::cout << std::endl;
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
		for (unsigned int i = 0; i < servers.size(); i++) {
			print_server(servers[i]);
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	
	
	std::cout << "OK" << std::endl;
}