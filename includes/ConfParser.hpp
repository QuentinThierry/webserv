#ifndef CONFPARSER_HPP
#define CONFPARSER_HPP

#include <iostream>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <exception>

#include "Server.hpp"

void	tokenize_file(std::fstream &s, std::queue<std::string> &tokens);
bool	wrap_getline_throw(std::fstream &s, std::string &buffer);

std::string	extract_token(std::queue<std::string> &tokens);
void	parse_tokens(std::queue<std::string> tokens);


void	interpret_tokens(std::queue<std::string> tokens, std::vector<Server> servers);

#endif