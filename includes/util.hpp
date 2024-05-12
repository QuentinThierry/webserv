#ifndef UTIL_HPP
# define UTIL_HPP
# include "Server.class.hpp"
# include <cstdio>

std::string error_message_server(Server const &server, std::string const &message);
uint64_t ft_atoi(std::string nb, e_status &error);
std::string ft_itoa( uint64_t number );

#endif