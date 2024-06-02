#ifndef UTILS_HPP
# define UTILS_HPP

# include "Server.class.hpp"
# include "Shared.hpp"

# include <sstream>
# include <fcntl.h>
# include <cstdio>

std::string		error_message_server(Server const &server, std::string const &message);
uint64_t 		ft_atoi(std::string nb, e_status &error);
std::string		ft_itoa( uint64_t number );
bool			is_accessible_directory(char const *name);
e_status 		empty_sstream_in_string(std::string &dest,
					std::stringstream &stream_to_empty);
uint64_t		str_to_hex(std::string str, bool &error);
std::string		hex_to_str(uint64_t val);


#endif