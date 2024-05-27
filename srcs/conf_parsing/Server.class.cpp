#include "Server.class.hpp"

Server::Server()
{
	_host = "";
	_host_uint = 0;
	_port = 0;
	_server_name.clear();
	_error_page_path.clear();
	_client_max_body_size = ONE_MEGABIT;
	_locations.clear();
	__has_listen = false;
}

std::string const 							&Server::getHost() const
													{return this->_host;}
uint32_t const 								&Server::getHostUint() const
													{return this->_host_uint;}
t_port const 								&Server::getPort() const
													{return this->_port;}
std::vector<std::string> const 				&Server::getServerName() const
													{return this->_server_name;}
std::vector<std::string> 					&Server::getServerName()
													{return this->_server_name;}
std::map<t_http_code, std::string> const 	&Server::getErrorPagePath() const
													{return this->_error_page_path;}
std::map<t_http_code, std::string> 			&Server::getErrorPagePath()
													{return this->_error_page_path;}
uint64_t const 								&Server::getClientMaxBodySize() const
													{return this->_client_max_body_size;}
std::vector<Location> const 				&Server::getLocations() const
													{return this->_locations;}
std::vector<CgiLocation> const 				&Server::getCgiLocations() const
													{return this->_cgi_locations;}
Location 									&Server::getDefaultLocation()
													{return this->_locations[0];}
Location const 								&Server::getDefaultLocation() const
													{return this->_locations[0];}
bool const 									&Server::getHasListen() const
													{return this->__has_listen;}

void	Server::setHost(std::string host)
			{this->_host = host;}
void	Server::setHostUint(uint32_t host_uint)
			{this->_host_uint = host_uint;}
void	Server::setServerName(std::vector<std::string> server_names)
			{this->_server_name = server_names;}
void	Server::addServerName(std::string server_name)
			{this->_server_name.push_back(server_name);}
void	Server::setPort(t_port port)
			{this->_port = port;}
void	Server::setErrorPagePath(std::map<t_http_code, std::string> error_page_path)
			{this->_error_page_path = error_page_path;}
void	Server::addErrorPagePath(t_http_code http_code, std::string path)
			{this->_error_page_path.insert(std::make_pair(http_code, path));}
void	Server::setClientMaxBodySize(uint64_t client_max_body_size)
			{this->_client_max_body_size = client_max_body_size;}
void	Server::setLocations(std::vector<Location> locations)
			{this->_locations = locations;}
void	Server::addLocations(Location location)
			{this->_locations.push_back(location);}
void	Server::addCgiLocation(CgiLocation cgi_loc)
			{this->_cgi_locations.push_back(cgi_loc);}
void	Server::_setHasListen(bool has_listen)
			{this->__has_listen = has_listen;}


// return if the server has the same host and port
bool	Server::is_equal(Server const &ref) const
{
	if (this->getHost() != ref.getHost())
		return false;
	if (this->getPort() != ref.getPort())
		return false;
	return true;
}

Location const &Server::searchLocation(std::string path) const
{
	size_t slash_pos = 0;

	// exact match
	for (unsigned int i = 1; i < this->getLocations().size(); i++)
	{
		if (this->getLocations()[i].getLocationPath() == path)
			return this->getLocations()[i];
	}
	// match directories
	do
	{
		for (unsigned int i = 1; i < this->getLocations().size(); i++)
		{
			if (this->getLocations()[i].getLocationPath() == path)
				return this->getLocations()[i];
		}
		if (path[path.size() - 1] == '/')
			path.erase(path.size() - 1);
		slash_pos = path.find_last_of('/');
		if (slash_pos != std::string::npos && path.size() - slash_pos > 1)
			path = path.substr(0, slash_pos + 1);
	} while (slash_pos != std::string::npos && path.size() != 0);

	return this->getDefaultLocation();
}

// returns true if a CgiLocation exists, and fills cgi_loc
// else, returns false and cgi_loc is left untouched
bool Server::searchCgiLocation(std::string path, CgiLocation &cgi_loc) const
{
	if (path[path.size() - 1] == '/')
		return false;
	size_t slash_pos = path.find_last_of('/');
	if (slash_pos != std::string::npos)
		path = path.substr(slash_pos + 1);
	size_t dot_pos = path.find_first_of('.');
	if (dot_pos == std::string::npos || path.size() == 1)
		return false;
	path = path.substr(dot_pos);
	
	for (unsigned int i = 0; i < this->getCgiLocations().size(); i++)
	{
		if (this->getCgiLocations()[i].getExtension() == path)
		{
			cgi_loc = this->getCgiLocations()[i];
			return true;
		}
	}
	return false;
}

std::string Server::getErrorPagePath(t_http_code error_code) const {
	std::map<t_http_code, std::string>::const_iterator it;
	it = this->_error_page_path.find(error_code);
	if (it != this->_error_page_path.end())
		return it->second;
	return "";
}
