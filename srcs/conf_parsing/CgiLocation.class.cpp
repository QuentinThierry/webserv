#include "CgiLocation.class.hpp"


CgiLocation::CgiLocation()
{
	this->_extension = "";
	this->_exec_path = "";
	this->_root_path = "";
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

std::string const &CgiLocation::getRootPath() const
{
	return this->_root_path;
}

void CgiLocation::setExtension(std::string extension)
{
	this->_extension = extension;
}

void CgiLocation::setExecPath(std::string exec_path)
{
	this->_exec_path = exec_path;
}

void CgiLocation::setRootPath(std::string root_path)
{
	this->_root_path = root_path;
}

void CgiLocation::check_validity() const
{
	if (this->_exec_path == "")
		ThrowMisc("missing exec_path argument");
	else if (this->_root_path == "")
		ThrowMisc("missing path_info argument");
}