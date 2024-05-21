#include "Cgi.class.hpp"

#define INFILE "infile"
#define OUTFILE "outfile"
#define COMMAND "./cgi.cgi"
#define ARGS NULL

void	set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds, Cgi &cgi, int *max_fd)
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	FD_ZERO(exceptfds);
	FD_SET(cgi.getReadPipe(), readfds);
	FD_SET(cgi.getWritePipe(), writefds);

	*max_fd = std::max(cgi.getWritePipe(), cgi.getReadPipe()) + 1;
}

int main()
{
	std::ifstream stream;

	stream.open("infile");

	Cgi cgi(COMMAND, stream);

	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	int test = 0;

	while (true)
	{
		int max_fd;
		struct timeval time = {1,0};
		set_fds(&readfds, &writefds, &exceptfds, cgi, &max_fd);
		int nb_fds = select(max_fd, &readfds, &writefds, NULL, &time);

		if (nb_fds != 0)
		{
			if (FD_ISSET(cgi.getReadPipe(), &readfds))
			{
				cgi.read();
			}
			if (FD_ISSET(cgi.getWritePipe(), &writefds))
			{
				if (test == 1)
					continue;
				if (cgi.write() == 1)
					test = 1;
			}
		}
	}
}
