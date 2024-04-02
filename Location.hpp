#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>

using namespace std;

class Location
{
	public:

	private:
		string			_path; // ?
		vector<string>	_accepted_methods;
		string			_root_path;
		bool			_has_redirect;
		string			_redirect_path;
		bool			_has_autoindex;
		string			_default_dir_path;
		bool			_has_cgi;
		string			_cgi_pass;
};

#endif