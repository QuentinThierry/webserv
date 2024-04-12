#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include "shared.hpp"
# include <stdint.h>
# include <vector>
// #include "Location.hpp"

class Server
{
    public:
		Server(){};
		Server(Server const &copy): _port(copy._port), _host(copy._host),
				_server_name(copy._server_name), _error_page_path(copy._error_page_path),
				_client_max_body_size(copy._client_max_body_size){};
		Server & operator=(Server const &){ return *this;};
		Server(int port, int host): _port(port), _host(host){};
        uint32_t const &getHost() const {return _host;};
        std::vector<std::string> const &getServerName() const {return _server_name;};
        uint16_t const &getPort() const {return _port;};
        std::string const &getErrorPagePath() const {return _error_page_path;};
        uint64_t const &getClientmaxBodySize() const {return _client_max_body_size;};


    private:
        uint32_t                _host;
        uint16_t                _port;
        std::vector<std::string> _server_name; // multiple server names ?
        std::string              _error_page_path;
        uint64_t                _client_max_body_size;
        // std::vector<Location>    _locations;
};

#endif