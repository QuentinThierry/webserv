#include "DefineVariableFields.hpp"

t_token_append_function define_token_var_function(std::string &token, Location *location)
{
	// find function that matches token
	if (token == "listen")
		return fill_listen;
	if (token == "server_name")
		return fill_server_name;
	if (token == "error_page")
		return fill_error_page;
	if (token == "client_max_body_size")
		return fill_client_max_body_size;
	if (token == "root")
		return fill_root_path;
	if (token == "autoindex")
		return fill_autoindex;
	// at this point, these fields can only be declare in a location
	if (!location)
		throw std::out_of_range("Token not in location : " + token);
	if (token == "limit_except")
		return fill_limit_except;
	if (token == "return")
		return fill_redirect;
	if (token == "index")
		return fill_default_dir_file;
	if (token == "upload_path")
		return fill_upload_path;
	throw std::out_of_range("unknown token " + token);
}