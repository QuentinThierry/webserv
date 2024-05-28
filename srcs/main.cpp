#include "ConfParser.hpp"
#include <csignal>
#include "Cluster.class.hpp"
#include "HttpTools.hpp"
#include "DefaultPath.hpp"

int const	g_err_log_fd = STDERR_FILENO;
std::vector<std::string>	g_http_methods;
std::vector<std::string>	g_http_versions;

void cltr_c(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "-------------CLTR C-------------" <<std::endl;
	}
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
		std::vector<Server> servers;
		std::signal(SIGINT, cltr_c);
		_init_available_http_methods_versions();
		servers = parse_config(get_config(argc, argv));
		Cluster web_server(servers);
		web_server.runServer();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
