#include "ConfParser.hpp"
#include <algorithm>
#include <cstdlib>
#include <arpa/inet.h>

#define MAX_BITS_CLIENT_MAX_SIZE 9223372036854775807u
#define MAX_BITS_CLIENT_MAX_SIZE_LEN 20
#define MAX_KBITS_CLIENT_MAX_SIZE 9007199254740991u
#define MAX_KBITS_CLIENT_MAX_SIZE_LEN 16
#define MAX_MBITS_CLIENT_MAX_SIZE 8796093022207u
#define MAX_MBITS_CLIENT_MAX_SIZE_LEN 13
#define LOCALHOST "localhost"
#define LOCALHOST_RESOLVE "127.0.0.1"

typedef union
{
	uint32_t	ip;
	struct
	{
		uint8_t		field1;
		uint8_t		field2;
		uint8_t		field3;
		uint8_t		field4;
	};
}	u_ip_uint32;

u_int16_t	str_to_short(std::string str);
void	fill_listen(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_server_name(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_error_page(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_client_max_body_size(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_limit_except(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_redirect(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_root_path(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_autoindex(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_default_dir_file(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_can_upload(std::string &token, Server &server, Location *location, unsigned int arg_counter);
void	fill_upload_path(std::string &token, Server &server, Location *location, unsigned int arg_counter);
