#include "Cgi.class.hpp"
#include <cstring>

// to replace with proper values from Http* files
#define READ_SIZE 100
#define WRITE_SIZE 100

#define ARGS NULL

Cgi::Cgi()
{
	this->_fd_file_in = -1;
	this->_pid = -1;
	this->_pipe_input[READ] = -1;
	this->_pipe_input[WRITE] = -1;
	this->_pipe_output[READ] = -1;
	this->_pipe_output[WRITE] = -1;
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

bool	Cgi::isAlive() const
{
	int dummy;
	if (this->getPid() == -1)
		return false;
	if (waitpid(this->getPid(), &dummy, WNOHANG) != 0)
		return false;
	return true;
}

// returns -1 on error, 0 if the cgi can receive more data, 1 if the input file has reach eof
int	Cgi::write() const
{
	char buffer[WRITE_SIZE + 1] = {0};

	ssize_t read_size = ::read(this->_fd_file_in, buffer, WRITE_SIZE);
	if (read_size < WRITE_SIZE)
	{
		::write(this->_pipe_input[WRITE], buffer, read_size);
		return 1;
	}
	else if (read_size < 0)
		return -1;
	else
		::write(this->_pipe_input[WRITE], buffer, read_size);
	return 0;
}

// returns -1 on error, 0 if the cgi can receive more data
int	Cgi::write(std::string to_write) const
{
	ssize_t size = ::write(this->getWritePipe(), to_write.c_str(), to_write.size());
	if (size <= 0)
		return -1;
	return 0;
}

ssize_t	Cgi::read(std::string &buffer) const
{
	char buf[READ_SIZE + 1] = {0};
	ssize_t size;
	size = ::read(this->_pipe_output[READ], buf, READ_SIZE);
	if (size < 0)
		return -1;
	if (size == 0)
		return 0;
	buffer = buf;
	return size;
}

/* Returns a non zero value on error, detailed under :
 * 0 -> everything ok
 * 1 -> execve failed, path is not good, 502 Bad Gateway
 * 2 -> pipe or fork error, 500 Internal Server Error
*/
int Cgi::_exec(std::string cgi_path, char const *file_name, std::string root_path)
{
	if (access(cgi_path.c_str(), X_OK))
		return 1;
	if (pipe(this->_pipe_input) == -1)
		return 2;
	if (pipe(this->_pipe_output))
	{
		close(this->_pipe_input[READ]);
		close(this->_pipe_input[WRITE]);
		return 2;
	}
	this->_pid = fork();
	if (this->_pid == -1)
	{
		close(this->_pipe_input[READ]);
		close(this->_pipe_input[WRITE]);
		close(this->_pipe_output[READ]);
		close(this->_pipe_output[WRITE]);
		return 2;
	}
	else if (this->_pid == 0) // child
	{
		// chdir(root_path.c_str()); // protect
		dup2(this->_pipe_input[READ], 0); // protect
		dup2(this->_pipe_output[WRITE], 1); // protect
		close(this->_pipe_input[READ]);
		close(this->_pipe_input[WRITE]);
		close(this->_pipe_output[READ]);
		close(this->_pipe_output[WRITE]);
		execve(cgi_path.c_str(),
			(char **)(char const * const []){cgi_path.c_str(), file_name, NULL},
			(char **)(char const * const []){NULL});
		std::cerr << "failed to execute cgi, exiting child" << std::endl;
		std::exit(1);
	}
	return 0;
}

int	Cgi::execPost(std::string cgi_path, std::string root_path)
{
	return this->_exec(cgi_path, NULL, root_path);
}

int Cgi::execGet(std::string cgi_path, std::string file_name, std::string root_path)
{
	this->_fd_file_in = open(file_name.c_str(), O_RDONLY);
	if (this->_fd_file_in < 0)
		return 2;
	return this->_exec(cgi_path, file_name.c_str(), root_path);
}

Cgi::~Cgi()
{
	if (this->_fd_file_in != -1)
		close(this->_fd_file_in);
	if (this->_pipe_input[READ] != -1)
		close(this->_pipe_input[READ]);
	if (this->_pipe_input[WRITE] != -1)
		close(this->_pipe_input[WRITE]);
	if (this->_pipe_output[READ] != -1)
		close(this->_pipe_output[READ]);
	if (this->_pipe_output[WRITE] != -1)
		close(this->_pipe_output[WRITE]);
	if (this->_pid > 0 && kill(this->_pid, 0))
		kill(this->_pid, SIGTERM);
}
