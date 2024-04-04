#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>

class Location
{
	public:
		std::string const &getLocationPath() const;
		std::vector<std::string> const &getMethods() const;
		std::string const &getRootPath() const;
		std::string const &getRedirectPath() const;
		std::string const &getDefaultDirPath() const;
		std::string const &getDefaultUploadPath() const;
		std::string const &getCgiPath() const;

		bool const &hasRedirect() const;
		bool const &hasAutoindex() const;
		bool const &canAcceptUpload() const;
		bool const &hasCgi() const;



	private:
		std::string			_location_path;
		std::vector<std::string>	_accepted_methods;
		std::string			_root_path;
		bool			_has_redirect;
		std::string			_redirect_path;
		std::string			_default_dir_path;
		bool			_has_autoindex;
		bool			_is_accept_upload;
		std::string			_upload_path;
		bool			_has_cgi;
		std::string			_cgi_path;
};

#endif