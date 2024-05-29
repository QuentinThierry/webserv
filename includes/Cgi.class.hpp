#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/wait.h>
#include "HttpExchange.class.hpp"

#define READ 0
#define WRITE 1
#define NB_ENV_VARIABLE 19

class Cgi
{
	public:
		Cgi();

		int getPid() const;
		int getWritePipe() const;
		int getReadPipe() const;

		int write() const;
		int write(std::string to_write) const;
		ssize_t read(std::string &buffer) const;
		int execGet(std::string cgi_path, std::string file_name, HttpExchange const &httpExchange);
		int	execPost(std::string cgi_path, HttpExchange const &httpExchange);
		bool isAlive() const;

		class NExceptionChildFail
		{
			
		};

		~Cgi();

	private:
		int _exec(std::string cgi_path, char const * file_name, HttpExchange const &httpExchange);

		int				_pipe_input[2];
		int				_pipe_output[2];
		pid_t			_pid;
};

#endif
