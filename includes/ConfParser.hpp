#ifndef CONFPARSER_HPP
#define CONFPARSER_HPP

#include <iostream>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <exception>

#include "Server.class.hpp"

typedef void (* t_token_append_function)(std::string &, Server &, Location *, unsigned int arg_counter);



void	tokenize_file(std::fstream &s, std::queue<std::string> &tokens);
bool	wrap_getline_throw(std::fstream &s, std::string &buffer);

std::string	extract_token(std::queue<std::string> &tokens);
void	parse_tokens(std::queue<std::string> tokens);

void	interpret_tokens(std::queue<std::string> &tokens, std::vector<Server> &servers);

t_token_append_function define_token_var_function(std::string &token);

bool	does_http_error_code_exist(std::string code);
uint16_t	http_error_code_to_uint16(std::string code);

#endif