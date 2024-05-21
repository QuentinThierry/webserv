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
		Cgi(std::string cgi_path, std::ifstream &input_file_stream);

		int getPid() const;
		int getWritePipe() const;
		int getReadPipe() const;

		void	set_file_in(std::ifstream &file_in);

		int write() const;
		std::string read() const;
		~Cgi();

	private:
		std::ifstream	*_file_in;
		int				_pipe_input[2];
		int				_pipe_output[2];
		pid_t			_pid;
};

#endif

/*

ifstream > p1W ----- (stdin/p1R < cgi > stdout/pipe2Write) ---- pipe2Read

main
    pipe1
    pipe2
    fork
    if pid == 0
    {
        dup2(stdin -> pipe1Read);
        dupe2(stdout -> pipe2Write);
        execve;
    }
    else
    {
        write(ifstream -> pipe1Write);
        read(pipe2Read); -> WriteSocket
    }
*/