#include "Cgi.class.hpp"
#include "HttpExchange.class.hpp"

Cgi::Cgi()
{
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

static char const **create_cgi_env(HttpExchange const &httpExchange, char const *file_name)
{
	std::string str;
	bool	is_get = false;
	char const **env;
	try
	{
		env = new char const *[NB_ENV_VARIABLE];
		env[11] = alloc_str("REQUEST_METHOD=" + *httpExchange.getRequest().getMethod()); // method
		if (*httpExchange.getRequest().getMethod() == "GET")
			is_get = true;

		env[0] = alloc_str("AUTH_TYPE=Basic"); // DEFAULT
		if (is_get)
		{
			if (httpExchange.getResponse().checkFieldExistence("Content-Length"))
			{
				std::vector<std::string> vec = httpExchange.getResponse().getFieldValue("Content-Length");
				if (vec.size() != 1)
					throw ExceptionHttpStatusCode(HTTP_500);
				env[1] = alloc_str("CONTENT_LENGTH=" + vec[0]);
			}
			else
				env[1] = alloc_str("");
		}
		else
		{
			if (httpExchange.getRequest().checkFieldExistence("Content-Length"))
			{
				std::vector<std::string> vec = httpExchange.getRequest().getFieldValue("Content-Length");
				if (vec.size() != 1)
					throw ExceptionHttpStatusCode(HTTP_500);
				env[1] = alloc_str("CONTENT_LENGTH=" + vec[0]);
			}
			else
				env[1] = alloc_str("");
		}

		if (!is_get && httpExchange.getRequest().checkFieldExistence("Content-Type"))
		{
			std::vector<std::string> vec = httpExchange.getRequest().getFieldValue("Content-Type");
			if (vec.size() != 1)
				throw ExceptionHttpStatusCode(HTTP_400);
			env[2] = alloc_str("CONTENT_TYPE=" + vec[0]);
		}
		else
			env[2] = alloc_str("");
		env[3]  = "GATEWAY_INTERFACE=CGI/1.1"; // DEFAULT
		env[4] = alloc_str("PATH_INFO=" + getUri(httpExchange.getSocket().getServer().searchLocation(httpExchange.getRequest().getTarget()).getRootPath(), httpExchange.getRequest().getTarget()));
		env[5]  = alloc_str(env[4]); // root/URI
		env[6]  = "QUERY_STRING=\"\""; // '?' arguments TODO
		env[7]  = alloc_str("REMOTE_ADDR=" + httpExchange.getSocket().getServer().getHost());
		env[8]  = alloc_str("REMOTE_HOST=" + httpExchange.getRequest().getFieldValue("Host")[0]);
		env[9]  = alloc_str("REMOTE_IDENT=none"); // identity information DEFAULT
		env[10] = alloc_str("REMOTE_USER=none"); // user identification DEFAULT (?)
		env[12] = alloc_str(std::string("SCRIPT_FILENAME=") + file_name); // URI path without root
		env[13] = alloc_str("SERVER_NAME="SERVER_NAME); // server name DEFAULT
		env[14] = alloc_str("SERVER_PORT=" + ft_itoa(httpExchange.getSocket().getServer().getPort())); // port request
		env[15] = alloc_str("SERVER_PROTOCOL=" + *httpExchange.getRequest().getVersion()); // http protocol version
		env[16] = alloc_str("SERVER_SOFTWARE=Unix"); // server OS of request TODO (in User-agent)
		env[17] = alloc_str("REDIRECT_STATUS=true"); // add to serve some cgi requirement (php-cgi) TRUE
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
int Cgi::_exec(std::string cgi_path, char const *file_name, std::string root_path, HttpExchange const &httpExchange)
{
	if (access(cgi_path.c_str(), X_OK))
		return 1;
	if (pipe(this->_pipe_input) == -1)
		return 2;
	if (pipe(this->_pipe_output) == -1)
		return 2;
	this->_pid = fork();
	if (this->_pid == -1)
		return 2;
	else if (this->_pid == 0) // child
	{
		bool error = false;
		if (dup2(this->_pipe_input[READ], 0) == -1) error = true;
		if (dup2(this->_pipe_output[WRITE], 1) == -1) error = true;
		close(this->_pipe_input[READ]);
		close(this->_pipe_input[WRITE]);
		close(this->_pipe_output[READ]);
		close(this->_pipe_output[WRITE]);
		if (!error)
		{
			char const **env = create_cgi_env(httpExchange, file_name);
			if (env)
			{
				execve(cgi_path.c_str(),
					(char **)(char const * const []){cgi_path.c_str(), file_name, NULL},
					(char **)env);
			}
		}
		throw Cgi::NExceptionChildFail();
	}
	return 0;
}

int	Cgi::execPost(std::string cgi_path, std::string root_path, HttpExchange const &httpExchange)
{
	return this->_exec(cgi_path, NULL, root_path, httpExchange);
}

int Cgi::execGet(std::string cgi_path, std::string file_name, std::string root_path, HttpExchange const &httpExchange)
{
	return this->_exec(cgi_path, file_name.c_str(), root_path, httpExchange);
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
	if (this->_pid > 0 && kill(this->_pid, 0))
		kill(this->_pid, SIGTERM);
}
