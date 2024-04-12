#include "DefineVariableFields.hpp"

t_token_append_function define_token_var_function(std::string &token)
{
	// find function that matches token
	if (token == "listen")
		return fill_listen;
	else if (token == "server_name")
		return fill_server_name;
	else if (token == "error_page")
		return fill_error_page;
	else if (token == "client_max_body_size")
		return fill_client_max_body_size;
	else if (token == "limit_except")
		return fill_limit_except;
	else if (token == "return")
		return fill_redirect;
	else if (token == "root")
		return fill_root_path;
	else if (token == "autoindex")
		return fill_autoindex;
	else if (token == "index")
		return fill_default_dir_file;
	else if (token == "upload_path")
		return fill_upload_path;
	else
		throw std::exception();
}