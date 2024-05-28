#ifndef SHARED_HPP
# define SHARED_HPP

typedef enum e_status
{
	SUCCESS,
	FAILURE
} e_status;

# include <string>
# include <iostream>
# include <fstream>
# include <exception>
# include <unistd.h>
# include <stdint.h>

extern int const g_err_log_fd;

int protected_write(int const fd, std::string const str);

#endif