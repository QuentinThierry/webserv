#include "Cgi.class.hpp"
#include <cstring>

Cgi::Cgi()
{
	this->_file_in = NULL;
	this->_pid = -1;
	this->_pipe_input[READ] = -1;
	this->_pipe_input[WRITE] = -1;
	this->_pipe_output[READ] = -1;
	this->_pipe_output[WRITE] = -1;
}

// schema of the internal pipe structure
// ifstream > piW ----- (stdin/p1R < cgi > stdout/poW) ---- poR
Cgi::Cgi(std::string cgi_path, std::ifstream &input_file_stream)
{
	this->_file_in = &input_file_stream;


	pipe(this->_pipe_input); // protect
	pipe(this->_pipe_output); // protect

	this->_pid = fork();
	if (this->_pid == -1)
	{
		// throw error fork
	}
	else if (this->_pid == 0) // child
	{
		dup2(this->_pipe_input[READ], 0);
		dup2(this->_pipe_output[WRITE], 1);
		close(this->_pipe_input[READ]);
		close(this->_pipe_input[WRITE]);
		close(this->_pipe_output[READ]);
		close(this->_pipe_output[WRITE]);
		input_file_stream.close();
		execve(cgi_path.c_str(),
			(char **)(char const * const []){cgi_path.c_str(), NULL},
			(char **)(char const * const []){NULL});
		std::cerr << "failed to execute cgi, exiting child" << std::endl;
		std::exit(1);
	}
}

int Cgi::getPid() const
{
	return this->_pid;
}

int Cgi::getWritePipe() const
{
	return this->_pipe_input[WRITE];
}

int Cgi::getReadPipe() const
{
	return this->_pipe_output[READ];
}

void	Cgi::set_file_in(std::ifstream &file_in)
{
	this->_file_in = &file_in;
}

Cgi::~Cgi()
{
	close(this->_pipe_input[READ]);
	close(this->_pipe_input[WRITE]);
	close(this->_pipe_output[READ]);
	close(this->_pipe_output[WRITE]);
}

// to replace with proper values from Http* files
#define READ_SIZE 100
#define WRITE_SIZE 100

// returns -1 on error, 0 if the cgi can receive more data, 1 if the input file has reach eof
int	Cgi::write() const
{
	char buffer[WRITE_SIZE + 1] = {0};

	this->_file_in->read(buffer, WRITE_SIZE);
	if (this->_file_in->eof())
	{
		::write(this->_pipe_input[WRITE], buffer, this->_file_in->gcount());
		return 1;
	}
	else if (this->_file_in->fail())
		return -1;
	else
		::write(this->_pipe_input[WRITE], buffer, this->_file_in->gcount());
	return 0;
}

std::string	Cgi::read() const
{
	char buffer[READ_SIZE + 1] = {0};
	::read(this->_pipe_output[READ], buffer, READ_SIZE);

	std::cout << buffer;
	return buffer;
}
