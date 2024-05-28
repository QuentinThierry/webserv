#include "ConfParser.hpp"
#include <signal.h>
#include "Cluster.class.hpp"
#include "HttpTools.hpp"
#include "DefaultPath.hpp"

int const	g_err_log_fd = STDERR_FILENO;
std::vector<std::string>	g_http_methods;
std::vector<std::string>	g_http_versions;

void cltr_c(int sig)
{
	(void)sig;
}

char const * get_config(int argc, char **argv)
{
	if (argc == 1)
		return DEFAULT_CONFIG;
	else if (argc == 2)
		return argv[1];
	else
		ThrowMisc("Invalid number of argument");
}

int main(int argc, char **argv)
{
	try
	{
		signal(SIGINT, cltr_c);
		_init_available_http_methods_versions();
		std::vector<Server> servers = parse_config(get_config(argc, argv));
		// for (unsigned int i = 0; i < servers.size(); i++) {
		// 	print_server(servers[i]);
		// }
		Cluster web_server(servers);
		web_server.runServer();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
