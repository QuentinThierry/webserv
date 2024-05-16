#include "ConfParser.hpp"
#include <arpa/inet.h>
#include <signal.h>
#include "Cluster.class.hpp"
#include "HttpTools.hpp"
void	print_server(Server &server);

int const	g_err_log_fd = STDERR_FILENO;
std::vector<std::string>	g_http_methods;
std::vector<std::string>	g_http_versions;

void cltr_c(int sig)
{
	(void)sig;
}

int main(int argc, char **argv)
{
	signal(SIGINT, cltr_c);
	_init_available_http_methods_versions();

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
		Cluster web_server(servers);
		web_server.runServer();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

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
		std::cout << "\tCan upload ? : " << loc.getCanUpload() << std::endl;
		std::cout << "\tUpload dir : " << loc.getUploadPath() << std::endl;
	}

	for (unsigned int i = 0; i < server.getCgiLocations().size(); i++)
	{
		std::cout << "CGI_location : " << std::endl;
		CgiLocation const &cgi_loc = server.getCgiLocations()[i];
		std::cout << "\tExtension : " << cgi_loc.getExtension() << std::endl;
		std::cout << "\tExec path : " << cgi_loc.getExecPath() << std::endl;
	}
	std::cout << std::endl;
}
