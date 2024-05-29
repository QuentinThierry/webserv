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
extern bool g_exit;

int protected_write(int const fd, std::string const str);

class ExceptionCltrC: public std::exception
{
	public:
		char const *what() const throw() {return "Interrupt signal received";}
};

#endif