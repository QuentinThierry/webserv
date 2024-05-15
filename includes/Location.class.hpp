#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <utility>
#include <stdint.h>
#include <algorithm>

typedef uint16_t t_http_code;
typedef uint16_t t_port;

class Location
{
	public:
		Location();
		Location(Location const &ref);

		Location &operator=(Location const &ref);
		bool	operator==(Location const &rref) const;

		std::string const &getLocationPath() const;
		std::vector<std::string> const &getMethods() const;
		std::vector<std::string> &getMethods();
		std::string const &getRootPath() const;
		bool const &getHasRedirect() const;
		std::pair<t_http_code, std::string> const &getRedirect() const;
		std::pair<t_http_code, std::string> &getRedirect();
		std::vector<std::string> const &getDefaultDirPath() const;
		bool const &getHasAutoindex() const;
		bool const &getCanUpload() const;
		std::string const &getUploadPath() const;

		void	setLocationPath(std::string location_path);
		void	setMethods(std::vector<std::string> methods);
		void	addMethods(std::string method);
		void	setRootPath(std::string root_path);
		void	setHasRedirect(bool has_redirect);
		void	setRedirect(std::pair<t_http_code, std::string> redirect);
		void	setDefaultDirPath(std::vector<std::string> default_path);
		void	addDefaultDirPath(std::string default_path);
		void	setHasAutoindex(bool has_auto_index);
		void	setCanUpload(bool can_upload);
		void	setUploadPath(std::string upload_path);

		bool	is_empty_location(Location &default_location) const;
		bool	does_accept_method(std::string method) const;


	private:
		std::string							_location_path;
		std::vector<std::string>			_accepted_methods;
		std::string							_root_path;
		bool								_has_redirect;
		std::pair<t_http_code, std::string>	_redirect;
		std::vector<std::string>			_default_dir_path;
		bool								_has_autoindex;
		bool								_can_upload;
		std::string							_upload_path;
};

#endif