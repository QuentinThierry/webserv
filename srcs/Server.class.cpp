#include "Server.class.hpp"

Server::Server()
{
	_host = "";
	_port = 0;
	_error_page_path = "";
	_client_max_body_size = 0;
}

std::string const &Server::getHost() const {return this->_host;}

uint16_t const &Server::getPort() const {return this->_port;}

std::vector<std::string> const &Server::getServerName() const {return this->_server_name;}

std::string const &Server::getErrorPagePath() const {return this->_error_page_path;}

uint64_t const &Server::getClientmaxBodySize() const {return this->_client_max_body_size;}

std::vector<Location> const &Server::getLocations() const {return this->_locations;}

Location &Server::getDefaultLocation() {return this->_locations[0];}

void	Server::setHost(std::string host) {this->_host = host;}

void	Server::setServerName(std::vector<std::string> server_names) {this->_server_name = server_names;}

void	Server::addServerName(std::string server_name) {this->_server_name.push_back(server_name);}

void	Server::setPort(uint16_t port) {this->_port = port;}

void	Server::setErrorPagePath(std::string error_page_path) {this->_error_page_path = error_page_path;}

void	Server::setClientmaxBodySize(uint64_t client_max_body_size) {this->_client_max_body_size = client_max_body_size;}

void	Server::setLocations(std::vector<Location> locations) {this->_locations = locations;}

void	Server::addLocations(Location location) {this->_locations.push_back(location);}
