#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include <stdint.h>

class Server
{
	public:
		std::string const &getHost() const;
		std::string const &getServerName() const;
		uint16_t const &getPort() const;
		std::string const &getErrorPagePath() const;
		uint64_t const &getClientmaxBodySize() const;

		void	setHost(std::string host);
		void	setServerName(std::vector<std::string> server_names);
		void	setServerName(std::string server_name);
		void	setPort(uint16_t port);
		void	setErrorPagePath(std::string error_page_path);
		void	setClientmaxBodySize(uint64_t client_max_body_size);

	private:
		std::string					_host;
		uint16_t					_port;
		std::vector<std::string>	_server_name;
		std::string					_error_page_path;
		uint64_t					_client_max_body_size;
		std::vector<Location>		_locations;
};

#endif