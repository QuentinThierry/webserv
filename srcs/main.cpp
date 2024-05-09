#include "Cluster.class.hpp"
#include "HttpTools.hpp"
#include <signal.h>
#include <cstdlib>

int const	g_err_log_fd = STDERR_FILENO;
std::vector<std::string>	g_http_methods;
std::vector<std::string>	g_http_versions;

int main()
{
	g_http_methods.push_back("GET");
	g_http_methods.push_back("POST");
	g_http_methods.push_back("DELETE");
	std::cout << g_http_methods.size() << std::endl;

	g_http_versions.push_back("HTTP");
	g_http_versions.push_back("HTTP/0.9");
	g_http_versions.push_back("HTTP/1.0");
	g_http_versions.push_back("HTTP/1.1");
	std::vector<Server> servers;
	Server first(8080, INADDR_ANY);
	servers.push_back(first);
	Server second(8081, INADDR_ANY);
	servers.push_back(second);
	Server third(1024, INADDR_ANY);
	servers.push_back(third);
	Cluster web_server(servers);
	web_server.runServer();
}