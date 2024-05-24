#include "Cgi.class.hpp"
#include <cstring>

#define INFILE "infile"
#define OUTFILE "outfile"
#define COMMAND "/usr/bin/php-cgi"

void	set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds, Cgi &cgi, int *max_fd, int do_it)
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	FD_ZERO(exceptfds);
	if (do_it == 0)
	{
		FD_SET(cgi.getReadPipe(), readfds);
		FD_SET(cgi.getWritePipe(), writefds);
		*max_fd = std::max(cgi.getWritePipe(), cgi.getReadPipe()) + 1;
	}
	else if (do_it == 1)
	{
		FD_SET(cgi.getReadPipe(), readfds);
		*max_fd = cgi.getReadPipe() + 1;
	}
	else
	{
		*max_fd = 0;
	}
}

#include <sys/ioctl.h>

int main()
{
	Cgi cgi;

	cgi.execPost(COMMAND, ".");

	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;



	int test = 0;
	int do_it = 0;
	int fd = open("phpfile.php", O_RDONLY);
	char buf[101];
	std::string test_str_write;
	ssize_t size = 1;
	while (size > 0)
	{
		memset(buf, 0, 101);
		size = read(fd, buf, 100);
		if (size > 0)
			test_str_write += buf;
	}
	close(fd);





	while (true)
	{
		int max_fd;
		struct timeval time = {1,0};
		set_fds(&readfds, &writefds, &exceptfds, cgi, &max_fd, do_it);
		int nb_fds = select(max_fd, &readfds, &writefds, NULL, &time);
		if (!cgi.isAlive())
		{
			return 0;
		}
		if (nb_fds != 0)
		{
			if (FD_ISSET(cgi.getReadPipe(), &readfds))
			{
				std::string buffer;
				ssize_t read_size = cgi.read(buffer);
				std::cout << buffer;
				if (read_size == 0)
					do_it = 2;
			}
			else if (FD_ISSET(cgi.getWritePipe(), &writefds))
			{
				if (test == 1)
					continue;
				int w = cgi.write("<?php ecdho \"coucou\";?>");
				if (w == 0)
				{
					test = 1;
					close(cgi.getWritePipe());
					do_it = 1;
				}
			}
		}
		else
			std::cerr << "no event" << std::endl;
	}
}

// check if upload path has a path after is on
