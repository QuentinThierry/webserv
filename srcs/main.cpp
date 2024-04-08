#include "Cluster.class.hpp"
#include <signal.h>
#include <cstdlib>

void test(int)
{
	std::exit(FAILURE);
}

int main()
{
	signal(SIGINT, test);
	std::vector<Server> servers;
	Server first(8080, INADDR_ANY);
	servers.push_back(first);
	Server second(8080, INADDR_ANY);
	servers.push_back(second);
	Server third(1023, INADDR_ANY);
	servers.push_back(third);
	Cluster web_server = Cluster(servers);
	web_server.runServer();
}