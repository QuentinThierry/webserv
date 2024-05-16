#include "CgiLocation.class.hpp"


CgiLocation::CgiLocation()
{
	this->_extension = "";
	this->_exec_path = "";
}

// do not check if _extension is equal
bool	CgiLocation::operator==(CgiLocation const &rref) const
{
	if (this->_exec_path == rref._exec_path)
		return true;
	return false;
}

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

bool CgiLocation::is_empty_cgi_location()
{
	
	if (*this == CgiLocation())
		return true;
	return false;
}