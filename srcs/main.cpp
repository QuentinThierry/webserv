#include "ConfParser.hpp"
#include <csignal>
#include "Cluster.class.hpp"
#include "HttpTools.hpp"
#include "DefaultPath.hpp"
#include "Cgi.class.hpp"

int	g_err_log_fd;
bool g_exit = false;

std::vector<std::string>			g_http_methods;
std::vector<std::string>			g_http_versions;
std::map<std::string, std::string>	g_http_content_type;

static void _cltr_c(int sig)
{
	if (sig == SIGINT && g_exit == false)
		g_exit = true;
}

static char const * get_config(int argc, char **argv)
{
	if (argc == 1)
		return DEFAULT_CONFIG;
	else if (argc == 2)
		return argv[1];
	else
		ThrowMisc("Invalid number of argument");
}

static void _open_log_file(int argc, char **argv)
{
	if (std::string(LOG_FILE_NAME).empty())
		g_err_log_fd = -1;
	else if (std::string(LOG_FILE_NAME) == "STDERR")
		g_err_log_fd = STDERR_FILENO;
	else if (std::string(LOG_FILE_NAME) == "STDOUT")
		g_err_log_fd = STDOUT_FILENO;
	else
	{
		g_err_log_fd = open(LOG_FILE_NAME, O_RDWR | O_APPEND | O_CREAT, 0662);
		if (g_err_log_fd == -1)
			std::cerr << WARNING_MSG_LOG_FAIL << std::endl;
	}
	protected_write_log("====== START ======");
	if (argc == 1)
		protected_write_log(std::string("Default configuration: ") + DEFAULT_CONFIG);
	else
		protected_write_log(std::string("Configuration file: ") + argv[1]);
}

static void _close_log_file()
{
	if (g_err_log_fd >= 0)
	{
		protected_write_log("====== END ======\n\n");
		close(g_err_log_fd);
	}
}

int main(int argc, char **argv)
{
	std::vector<Server> servers;
	std::srand(time(NULL));
	std::signal(SIGINT, _cltr_c);
	_open_log_file(argc, argv);
	try
	{
		std::cout << "Server(s) configuration..." << std::endl;
		_init_available_http_methods_versions_type();
		servers = parse_config(get_config(argc, argv));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		_close_log_file();
		return 1;
	}
	std::cout << "Server(s) running..." << std::endl;
	while (g_exit == false)
	{
		try
		{
			Cluster web_server(servers);
			web_server.runServer();
		}
		catch (Cgi::NExceptionChildFail &e)
		{
			_close_log_file();
			return 0;
		}
		catch (std::exception &e)
		{
			protected_write_log(e.what());
		}
	}
	_close_log_file();
	return 0;
}
