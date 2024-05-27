#include "ConfParser.hpp"
#include <arpa/inet.h>
#include <signal.h>


#include "Cluster.class.hpp"
#include "HttpTools.hpp"
#include "Autoindex.class.hpp"

int const	g_err_log_fd = STDERR_FILENO;
std::vector<std::string>	g_http_methods;
std::vector<std::string>	g_http_versions;

int main()
{
	std::string body;

	Autoindex index("/mnt/nfs/homes/acardona/Documents/Webserv_perso/webserv");

	body = index.generateAutoIndexBody();
	std::cout << body;

	return 0;
}