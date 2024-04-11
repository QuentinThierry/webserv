#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.class.hpp"
#include <stdint.h>
#include <map>

class Server
{
	public:
		Server();
		std::string const &getHost() const;
		uint16_t const &getPort() const;
		std::vector<std::string> const &getServerName() const;
		std::map<uint16_t, std::string> const &getErrorPagePath() const;
		std::map<uint16_t, std::string> &getErrorPagePath();
		uint64_t const &getClientmaxBodySize() const;
		std::vector<Location> const &getLocations() const;
		Location &getDefaultLocation();

		void	setHost(std::string host);
		void	setServerName(std::vector<std::string> server_names);
		void	addServerName(std::string server_name);
		void	setPort(uint16_t port);
		void	setErrorPagePath(std::map<uint16_t, std::string> error_page_path);
		void	addErrorPagePath(uint16_t http_code, std::string path);
		void	setClientmaxBodySize(uint64_t client_max_body_size);
		void	setLocations(std::vector<Location> locations);
		void	addLocations(Location location);

	private:
		std::string						_host;
		uint16_t						_port;
		std::vector<std::string>		_server_name;
		std::map<uint16_t, std::string>	_error_page_path;
		uint64_t						_client_max_body_size;
		std::vector<Location>			_locations;
};

#endif