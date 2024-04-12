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
		t_port const &getPort() const;
		std::vector<std::string> const &getServerName() const;
		std::vector<std::string> &getServerName();
		std::map<t_http_code, std::string> const &getErrorPagePath() const;
		std::map<t_http_code, std::string> &getErrorPagePath();
		uint64_t const &getClientmaxBodySize() const;
		std::vector<Location> const &getLocations() const;
		Location &getDefaultLocation();
		bool	&_getHasFoundErrorPagePath();
		bool	&_getHasListen();

		void	setHost(std::string host);
		void	setServerName(std::vector<std::string> server_names);
		void	addServerName(std::string server_name);
		void	setPort(t_port port);
		void	setErrorPagePath(std::map<t_http_code, std::string> error_page_path);
		void	addErrorPagePath(t_http_code http_code, std::string path);
		void	setClientmaxBodySize(uint64_t client_max_body_size);
		void	setLocations(std::vector<Location> locations);
		void	addLocations(Location location);
		void	_setHasFoundErrorPagePath(bool has_found_error_page_path);
		void	_setHasListen(bool has_listen);

		bool	is_equal(Server const &ref);

	private:
		std::string							_host;
		t_port								_port;
		std::vector<std::string>			_server_name;
		std::map<t_http_code, std::string>	_error_page_path;
		uint64_t							_client_max_body_size;
		std::vector<Location>				_locations;
		bool								__has_found_error_page_path;
		bool								__has_listen;
};

#endif