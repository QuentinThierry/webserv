#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

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
		int execGet(std::string cgi_path, std::string file_name, std::string root_path);
		int execPost(std::string cgi_path, std::string root_path);
		bool isAlive() const;

		~Cgi();

	private:
		int _exec(std::string cgi_path, char const * file_name, std::string root_path);

		int				_fd_file_in;
		int				_pipe_input[2];
		int				_pipe_output[2];
		pid_t			_pid;
};

#endif
