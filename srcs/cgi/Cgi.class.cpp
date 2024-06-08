#include "Cgi.class.hpp"
#include "HttpRequest.class.hpp"
#include "HttpRequestPost.class.hpp"
#include "HttpRequestGet.class.hpp"
#include "HttpRequestHead.class.hpp"
#include "Cluster.class.hpp"

Cgi::Cgi()
{
	this->_pid = -1;
	this->_pipe_input[READ] = -1;
	this->_pipe_input[WRITE] = -1;
	this->_pipe_output[READ] = -1;
	this->_pipe_output[WRITE] = -1;
	this->_cluster = NULL;
}

Cgi::Cgi(Cluster &cluster)
{
	this->_pid = -1;
	this->_pipe_input[READ] = -1;
	this->_pipe_input[WRITE] = -1;
	this->_pipe_output[READ] = -1;
	this->_pipe_output[WRITE] = -1;
	_cluster = &cluster;
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
	buffer = std::string(buf, size);
	return size;
}

void	Cgi::endWrite()
{
	close(getWritePipe());
	_pipe_input[WRITE] = -1;
}

void	Cgi::endRead()
{
	close(getReadPipe());
	_pipe_output[READ] = -1;
}

static char const *alloc_str(std::string str)
{
	unsigned int size = str.size();
	char *res = new char[size + 1];

	for (unsigned int i = 0; i < size; i++)
	{
		res[i] = str[i];
	}
	res[size] = 0;
	return res;
}

static void free_env(char const **env)
{
	for (unsigned int i = 0; i < NB_ENV_VARIABLE; i++)
	{
		delete[] env[i];
	}
	delete[] env;
}

static char const **create_cgi_env(HttpRequest const &request, Server const &server, std::string file_name)
{
	std::string str;
	bool	is_get = false;
	char const **env;
	try
	{
		env = new char const *[NB_ENV_VARIABLE];
		env[11] = alloc_str("REQUEST_METHOD=" + *request.getMethod());
		if (*request.getMethod() == "GET" || *request.getMethod() == "HEAD")
			is_get = true;

		env[0] = alloc_str("AUTH_TYPE=Basic");
		if (is_get)
		{
			env[1] = alloc_str("");
		}
		else
		{
			if (dynamic_cast<HttpRequestPost const &>(request).hasContentLength()
					&& request.checkFieldExistence("Content-Length"))
			{
				std::vector<std::string> vec = request.getFieldValue("Content-Length");
				if (vec.size() != 1)
					throw ExceptionHttpStatusCode(HTTP_500);
				env[1] = alloc_str("CONTENT_LENGTH=" + vec[0]);
			}
			else
				env[1] = alloc_str("");
		}

		if (!is_get && request.checkFieldExistence("Content-Type"))
		{
			std::vector<std::string> vec = request.getFieldValue("Content-Type");
			if (vec.size() != 1)
				throw ExceptionHttpStatusCode(HTTP_400);
			env[2] = alloc_str("CONTENT_TYPE=" + vec[0]);
		}
		else
			env[2] = alloc_str("");
		env[3]  = alloc_str("GATEWAY_INTERFACE=CGI/1.1");
		env[4] = alloc_str("PATH_INFO=" + file_name);
		env[5]  = alloc_str("PATH_TRANSLATED=" + file_name);
		if (is_get && *request.getMethod() == "GET")
			env[6]  = alloc_str("QUERY_STRING=" + dynamic_cast<HttpRequestGet const &>(request).getQueryString());
		else if (is_get && *request.getMethod() == "HEAD")
			env[6]  = alloc_str("QUERY_STRING=" + dynamic_cast<HttpRequestHead const &>(request).getQueryString());
		else
			env[6]  = alloc_str("QUERY_STRING=\"\"");
		env[7]  = alloc_str("REMOTE_ADDR=" + server.getHost());
		env[8]  = alloc_str("REMOTE_HOST=" + request.getFieldValue("Host")[0]);
		env[9]  = alloc_str("REMOTE_IDENT=none");
		env[10] = alloc_str("REMOTE_USER=none");
		env[12] = alloc_str("SCRIPT_FILENAME=" + file_name);
		env[13] = alloc_str("SERVER_NAME=" SERVER_NAME);
		env[14] = alloc_str("SERVER_PORT=" + ft_itoa(server.getPort()));
		env[15] = alloc_str("SERVER_PROTOCOL=" + *request.getVersion());
		env[16] = alloc_str("SERVER_SOFTWARE=Unix");
		env[17] = alloc_str("REDIRECT_STATUS=true");
		env[18] = NULL;
	}
	catch (std::exception &e)
	{
		return NULL;
	}
	return env;
}

/* Returns a non zero value on error, detailed under :
 * 0 -> everything ok
 * 1 -> execve failed, path is not good, 502 Bad Gateway
 * 2 -> pipe or fork error, 500 Internal Server Error
*/
void Cgi::exec(std::string cgi_path, std::string file_name, HttpRequest const &request, Server const &server)
{
	if (access(cgi_path.c_str(), X_OK))
		throw ExceptionHttpStatusCode(HTTP_502);
	if (pipe(this->_pipe_input) == -1)
		throw ExceptionHttpStatusCode(HTTP_500);
	if (pipe(this->_pipe_output) == -1)
		throw ExceptionHttpStatusCode(HTTP_500);
	this->_pid = fork();
	if (this->_pid == -1)
		throw ExceptionHttpStatusCode(HTTP_500);
	else if (this->_pid == 0)
	{
		bool error = false;
		if (dup2(this->_pipe_input[READ], 0) == -1) error = true;
		if (dup2(this->_pipe_output[WRITE], 1) == -1) error = true;
		close(this->_pipe_input[READ]);
		_pipe_input[READ] = -1;
		close(this->_pipe_input[WRITE]);
		_pipe_input[WRITE] = -1;
		close(this->_pipe_output[READ]);
		_pipe_output[READ] = -1;
		close(this->_pipe_output[WRITE]);
		_pipe_output[WRITE] = -1;
		if (!error)
		{
			char const **env = create_cgi_env(request, server, file_name);
			if (env)
			{
				char const *args[3] = {0};
				try
				{
					args[0] = alloc_str(cgi_path);
					args[1] = alloc_str(file_name);
					args[2] = NULL;
				}
				catch (std::exception &e)
				{
					free_env(env);
					if (args[0])
						delete [] args[0];
					if (args[1])
						delete [] args[1];
					throw Cgi::NExceptionChildFail();
				}
				if (_cluster != NULL)
					_cluster->clear();
				close(g_err_log_fd);
				g_err_log_fd = -1;
				execve(cgi_path.c_str(), (char * const *)args,
					(char **)env);
				free_env(env);
				delete [] args[0];
				delete [] args[1];
			}
		}
		throw Cgi::NExceptionChildFail();
	}
	else
	{
		close(_pipe_input[READ]);
		_pipe_input[READ] = -1;
		close(_pipe_output[WRITE]);
		_pipe_output[WRITE] = -1;
	}
}

Cgi::~Cgi()
{
	if (this->_pipe_input[READ] != -1)
		close(this->_pipe_input[READ]);
	if (this->_pipe_input[WRITE] != -1)
		close(this->_pipe_input[WRITE]);
	if (this->_pipe_output[READ] != -1)
		close(this->_pipe_output[READ]);
	if (this->_pipe_output[WRITE] != -1)
		close(this->_pipe_output[WRITE]);
	if (this->_pid > 0 && kill(this->_pid, 0) == 0)
	{
		if (kill(this->_pid, SIGTERM) == -1)
			protected_write_log("ERROR: cgi: kill fail");
	}
}
