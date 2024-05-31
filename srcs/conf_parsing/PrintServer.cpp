#include "Server.class.hpp"

void	print_server(Server &server)
{
	std::cout << "Host : " << server.getHost() << std::endl;
	std::cout << "Host uint : " << server.getHostUint() << std::endl;
	std::cout << "Port : " << server.getPort() << std::endl;
	std::cout << "Server_names : " << std::endl;
	for (unsigned int i = 0; i < server.getServerName().size(); i++) {
		std::cout << "\t" << server.getServerName()[i] << std::endl;
	}
	std::cout << "Error path : " << std::endl;
	for (std::map<t_http_code, std::string>::iterator it = server.getErrorPagePath().begin(); it != server.getErrorPagePath().end(); it++) {
		std::cout << "\t" << it->first << " " << it->second << std::endl;
	}
	std::cout << "Client mbs : " << server.getClientMaxBodySize() << std::endl;

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
		for (unsigned int j = 0; j < loc.getDefaultIndexPath().size(); j++) {
			std::cout << "\t\t" << loc.getDefaultIndexPath()[j] << std::endl;
		}
		std::cout << "\tHas autoindex : " << loc.getHasAutoindex() << std::endl;
		std::cout << "\tCan upload ? : " << loc.getCanUpload() << std::endl;
	}

	for (unsigned int i = 0; i < server.getCgiLocations().size(); i++)
	{
		std::cout << "CGI_location : " << std::endl;
		CgiLocation const &cgi_loc = server.getCgiLocations()[i];
		std::cout << "\tExtension : " << cgi_loc.getExtension() << std::endl;
		std::cout << "\tExec path : " << cgi_loc.getExecPath() << std::endl;
		std::cout << "\tRoot path : " << cgi_loc.getRootPath() << std::endl;
	}
	std::cout << std::endl;
}