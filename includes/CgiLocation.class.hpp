#ifndef CGILOCATION_HPP
#define CGILOCATION_HPP

#include <string>

class CgiLocation
{
	public:
		CgiLocation();
		bool	operator==(CgiLocation const &rref) const;

		std::string const &getExtension() const;
		std::string const &getExecPath() const;
		void setExtension(std::string extension);
		void setExecPath(std::string exec_path);

		bool is_empty_cgi_location();

	private:
		std::string _extension;
		std::string _exec_path; 
};

#endif