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

extern int const	g_err_log_fd;
extern bool 		g_exit;

// randomized errors path
#define RANDOMIZED_ERROR_PAGE_PATHS {\
	"cat0.webp", "cat1.webp", "cat2.webp", "cat3.webp",\
	"cat4.webp", "cat5.webp", "cat6.webp"\
}

// fixed errors path
#define ERROR_404_PATH "404error.webp"
#define ERROR_403_PATH "403forbidden.webp"
#define ERROR_418_PATH "418teapot.webp"
#define ERROR_500_PATH "500internalerror.webp"

int protected_write(int const fd, std::string const str);

class ExceptionCltrC: public std::exception
{
	public:
		char const *what() const throw() {return "Interrupt signal received";}
};

#endif