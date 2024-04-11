#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <utility>
#include <stdint.h>

class Location
{
	public:
		Location();

		std::string const &getLocationPath() const;
		std::vector<std::string> const &getMethods() const;
		std::vector<std::string> &getMethods();
		std::string const &getRootPath() const;
		bool const &getHasRedirect() const;
		std::pair<uint16_t, std::string> const &getRedirect() const;
		std::vector<std::string> const &getDefaultDirPath() const;
		bool const &getHasAutoindex() const;
		bool const &getCanAcceptUpload() const;
		std::string const &getDefaultUploadPath() const;
		bool const &getHasCgi() const;
		std::string const &getCgiPath() const;

		void	setLocationPath(std::string location_path);
		void	setMethods(std::vector<std::string> methods);
		void	addMethods(std::string method);
		void	setRootPath(std::string root_path);
		void	setHasRedirect(bool has_redirect);
		void	setRedirectPath(std::pair<uint16_t, std::string> redirect);
		void	setDefaultDirPath(std::vector<std::string> default_path);
		void	addDefaultDirPath(std::string default_path);
		void	setHasAutoindex(bool has_auto_index);
		void	setCanAcceptUpload(bool can_accept_upload);
		void	setDefaultUploadPath(std::string upload_path);
		void	setHasCgi(bool has_cgi);
		void	setCgiPath(std::string cgi_path);



	private:
		std::string							_location_path;
		std::vector<std::string>			_accepted_methods;
		std::string							_root_path;
		bool								_has_redirect;
		std::pair<uint16_t, std::string>	_redirect;
		std::vector<std::string>			_default_dir_path;
		bool								_has_autoindex;
		bool								_is_accept_upload;
		std::string							_upload_path;
		bool								_has_cgi;
		std::string							_cgi_path;
};

#endif