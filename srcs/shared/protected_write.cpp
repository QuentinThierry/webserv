#include "Shared.hpp"
#include "utils.hpp"

int protected_write(int const fd, std::string const str)
{
	int	n = 0;

	if (fd >= 0)
	{
		n = write(fd, str.c_str(), str.size());
		write(fd, "\n", 1);
	}
	return (n);
}

std::string error_message_server(Server const &server, std::string const &message)
{
	return message + " " + server.getHost() + ":" + ft_itoa(server.getPort()) + "\n";
}
