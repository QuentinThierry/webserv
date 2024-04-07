#ifndef WEBCONF_HPP
#define WEBCONF_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <exception>

void	tokenize_file(std::fstream &s, std::queue<std::string> &tokens);
bool	wrap_getline_throw(std::fstream &s, std::string &buffer);

#endif