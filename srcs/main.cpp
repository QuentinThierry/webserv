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
	if (sig == SIGINT)
		throw ExceptionCltrC();
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
	std::vector<Server> servers;
	signal(SIGINT, cltr_c);
	_init_available_http_methods_versions();
	try
	{
		servers = parse_config(get_config(argc, argv));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	while(1)
	{
		try
		{
			Cluster web_server(servers);
			web_server.runServer();
		}
		catch (ExceptionCltrC &e)
		{
			std::cout << e.what() << std::endl;
			return 1;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}
