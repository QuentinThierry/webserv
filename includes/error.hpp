#ifndef ERROR_HPP
# define ERROR_HPP
# include "Server.class.hpp"
# include <cstdio>

std::string error_message_server(Server const &server, std::string const &message);

#endif