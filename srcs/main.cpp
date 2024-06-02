#include "ConfParser.hpp"
#include <csignal>
#include "Cluster.class.hpp"
#include "HttpTools.hpp"
#include "DefaultPath.hpp"
#include "Cgi.class.hpp"

int const	g_err_log_fd = STDERR_FILENO;
bool g_exit = false;

std::vector<std::string>	g_http_methods;
std::vector<std::string>	g_http_versions;

void cltr_c(int sig)
{
	if (sig == SIGINT && g_exit == false)
		g_exit = true;
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
	std::srand(time(NULL));
	std::signal(SIGINT, cltr_c);
	try
	{
		_init_available_http_methods_versions();
		servers = parse_config(get_config(argc, argv));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	while (g_exit == false)
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
		catch (Cgi::NExceptionChildFail &e)
		{
			return 0;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return 0;
}
