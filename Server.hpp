#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"

class Server
{
	public:
		std::string const &getHost() const;
		std::string const &serverName() const;
		uint16_t const &getPort() const;
		std::string const &getErrorPagePath() const;
		uint64_t const &getClientmaxBodySize() const;


	private:
		std::string				_host;
		uint16_t				_port;
		std::string				_server_name; // multiple server names ?
		std::string				_error_page_path;
		uint64_t				_client_max_body_size;
		std::vector<Location>	_locations;
};

#endif