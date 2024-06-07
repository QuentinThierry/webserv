#include "Shared.hpp"
#include "utils.hpp"

int protected_write(int const fd, std::string const & str)
{
	int	n = 0;

	if (fd >= 0)
	{
		n = write(fd, str.c_str(), str.size());
		if (n >= 0)
			(write(fd, "\n", 1) == 1) ? ++n:n = -1 ;
	}
	return (n);
}

int protected_write_no_endl(int const fd, std::string const & str)
{
	int	n = 0;

	if (fd >= 0)
	{
		n = write(fd, str.c_str(), str.size());
	}
	return (n);
}


int protected_write_log(std::string const str)
{
	return (protected_write(g_err_log_fd, str));
}

int protected_write_log_no_endl(std::string const str)
{
	return (protected_write_no_endl(g_err_log_fd, str));
}


std::string error_message_server(Server const &server, std::string const &message)
{
	return message + " " + server.getHost() + ":" + ft_itoa(server.getPort()) + "\n";
}
