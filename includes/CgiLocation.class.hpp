#ifndef CGILOCATION_HPP
#define CGILOCATION_HPP

#include <string>
#include "ConfException.class.hpp"

class CgiLocation
{
	public:
		CgiLocation();
		bool	operator==(CgiLocation const &rref) const;

		std::string const &getExtension() const;
		std::string const &getExecPath() const;
		std::string const &getRootPath() const;
		void setExtension(std::string extension);
		void setExecPath(std::string exec_path);
		void setRootPath(std::string root_path);

		bool is_empty_cgi_location();
		void check_validity() const;

	private:
		std::string _extension;
		std::string _exec_path;
		std::string _root_path;
};

#endif