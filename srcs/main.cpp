#include "Cluster.class.hpp"
#include <signal.h>
#include <cstdlib>

extern int const g_err_log_fd;

int main()
{
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