#include "Location.class.hpp"

Location::Location()
{
	_location_path = "";
	_accepted_methods.push_back("GET");
	_accepted_methods.push_back("POST");
	_accepted_methods.push_back("DELETE");
	_root_path = "";
	_has_redirect = false;
	_redirect = std::pair<int, std::string>();
	_has_autoindex = false;
	_is_accept_upload = false;
	_upload_path = "";
	_has_cgi = false;
	_cgi_path = "";
}

std::string const &Location::getLocationPath() const {return this->_location_path;}
std::vector<std::string> const &Location::getMethods() const {return this->_accepted_methods;}
std::vector<std::string> &Location::getMethods() {return this->_accepted_methods;}
std::string const &Location::getRootPath() const {return this->_root_path;}
bool const &Location::getHasRedirect() const {return this->_has_redirect;}
std::pair<uint16_t, std::string> const &Location::getRedirect() const {return this->_redirect;}
std::vector<std::string> const &Location::getDefaultDirPath() const {return this->_default_dir_path;}
bool const &Location::getHasAutoindex() const {return this->_has_autoindex;}
bool const &Location::getCanAcceptUpload() const {return this->_is_accept_upload;}
std::string const &Location::getDefaultUploadPath() const {return this->_upload_path;}
bool const &Location::getHasCgi() const {return this->_has_cgi;}
std::string const &Location::getCgiPath() const {return this->_cgi_path;}

void	Location::setLocationPath(std::string location_path) {this->_location_path = location_path;}
void	Location::setMethods(std::vector<std::string> methods) {this->_accepted_methods = methods;}
void	Location::addMethods(std::string method) {this->_accepted_methods.push_back(method);}
void	Location::setRootPath(std::string root_path) {this->_root_path = root_path;}
void	Location::setHasRedirect(bool has_redirect) {this->_has_redirect = has_redirect;}
void	Location::setRedirectPath(std::pair<uint16_t, std::string> redirect) {this->_redirect = redirect;}
void	Location::setDefaultDirPath(std::vector<std::string> default_path) {this->_default_dir_path = default_path;}
void	Location::addDefaultDirPath(std::string default_path) {this->_default_dir_path.push_back(default_path);}
void	Location::setHasAutoindex(bool has_auto_index) {this->_has_autoindex = has_auto_index;}
void	Location::setCanAcceptUpload(bool can_accept_upload) {this->_is_accept_upload = can_accept_upload;}
void	Location::setDefaultUploadPath(std::string upload_path) {this->_upload_path = upload_path;}
void	Location::setHasCgi(bool has_cgi) {this->_has_cgi = has_cgi;}
void	Location::setCgiPath(std::string cgi_path) {this->_cgi_path = cgi_path;}