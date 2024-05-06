#include "shared.hpp"
#include "util.hpp"

int protected_write(int const fd, std::string const str)
{
	return (write(fd, str.c_str(), str.length()));
}

std::string error_message_server(Server const &server, std::string const &message)
{
	char error[1000]; 	//! need to changes size after
	sprintf(error, "%d:%d",server.getHost(),server.getPort()); //! remove host after
	return message + " " + error + "\n";
}
