#include "Server.class.hpp"

Server::Server()
{
	_host = "";
	_port = 0;
	_server_name = std::vector<std::string>();
	_error_page_path = std::map<t_http_code, std::string>();
	_client_max_body_size = 0;
	_locations = std::vector<Location>();
	__has_found_error_page_path = false;
	__has_listen = false;
}

std::string const &Server::getHost() const {return this->_host;}
t_port const &Server::getPort() const {return this->_port;}
std::vector<std::string> const &Server::getServerName() const {return this->_server_name;}
std::vector<std::string> &Server::getServerName() {return this->_server_name;}
std::map<t_http_code, std::string> const &Server::getErrorPagePath() const {return this->_error_page_path;}
std::map<t_http_code, std::string> &Server::getErrorPagePath() {return this->_error_page_path;}
uint64_t const &Server::getClientmaxBodySize() const {return this->_client_max_body_size;}
std::vector<Location> const &Server::getLocations() const {return this->_locations;}
Location &Server::getDefaultLocation() {return this->_locations[0];}
bool	&Server::_getHasFoundErrorPagePath() {return this->__has_found_error_page_path;}
bool	&Server::_getHasListen() {return this->__has_listen;}

void	Server::setHost(std::string host) {this->_host = host;}
void	Server::setServerName(std::vector<std::string> server_names) {this->_server_name = server_names;}
void	Server::addServerName(std::string server_name) {this->_server_name.push_back(server_name);}
void	Server::setPort(t_port port) {this->_port = port;}
void	Server::setErrorPagePath(std::map<t_http_code, std::string> error_page_path) {this->_error_page_path = error_page_path;}
void	Server::addErrorPagePath(t_http_code http_code, std::string path) {this->_error_page_path.insert(std::make_pair(http_code, path));}
void	Server::setClientmaxBodySize(uint64_t client_max_body_size) {this->_client_max_body_size = client_max_body_size;}
void	Server::setLocations(std::vector<Location> locations) {this->_locations = locations;}
void	Server::addLocations(Location location) {this->_locations.push_back(location);}
void	Server::_setHasFoundErrorPagePath(bool has_found_error_page_path) {this->__has_found_error_page_path = has_found_error_page_path;}
void	Server::_setHasListen(bool has_listen) {this->__has_listen = has_listen;}

// return if the server has the same host and port
bool	Server::is_equal(Server const &ref)
{
	if (this->getHost() != ref.getHost())
		return false;
	if (this->getPort() != ref.getPort())
		return false;
	return true;
}