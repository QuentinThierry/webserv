#include "CgiLocation.class.hpp"

std::string const &CgiLocation::getExtension() const
{
	return this->_extension;
}

std::string const &CgiLocation::getExecPath() const
{
	return this->_exec_path;
}

void CgiLocation::setExtension(std::string extension)
{
	this->_extension = extension;
}

void CgiLocation::setExecPath(std::string exec_path)
{
	this->_exec_path = exec_path;
}
