#ifndef CGILOCATION_HPP
#define CGILOCATION_HPP

#include <string>

class CgiLocation
{
	public:
		std::string const &getExtension() const;
		std::string const &getExecPath() const;
		void setExtension(std::string extension);
		void setExecPath(std::string exec_path);

	private:
		std::string _extension;
		std::string _exec_path; 
};

#endif