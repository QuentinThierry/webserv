#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include "Location.hpp"

using namespace std;

class Server
{
	public:

	private:
		string		_host;
		uint16_t	_port;
		string		_server_name;
		string		_error_page_path;
		uint64_t	_client_max_body_size;
		Location	_location;
};

#endif