#ifndef SHARED_HPP
# define SHARED_HPP


# include <string>
# include <iostream>
# include <fstream>
# include <exception>
# include <unistd.h>
# include <stdint.h>
# include <vector>

# ifndef LOG_FILE_NAME
#  define LOG_FILE_NAME "webserv.log"
# endif

# define WARNING_MSG_LOG_FAIL "WARNING: Server: Fail to open the log file, running without log" 

// randomized errors path
#define RANDOMIZED_ERROR_PAGE_PATHS {\
	"https://blog.petsplanet.it/media/k.jpg",\
	"https://www.zoomalia.com/blogz/3988/taurine-chat-bienfaits-sante-conseils-experts.jpeg",\
	"https://cloudfront-eu-central-1.images.arcpublishing.com/lexpress/VQ6QDWLMKFGCXFORD36RA6RZ7I.jpg",\
	"https://i.imgflip.com/qv8cu.jpg?a476784",\
	"https://encrypted-tbn2.gstatic.com/images?q=tbn:ANd9GcQVJP5IFMmFrvK3vbtoLgKdvCo_H_5NavMJdG2WISny4iOLWpjO",\
	"https://img-9gag-fun.9cache.com/photo/aDD4PdK_700bwp.webp",\
	"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQk7MUMQMcWdUsbLlolvhorSaYfARIE5Zh-lsJ_kI06yP5kiK4cbqyJezjPiV1Xasnfi4Q&usqp=CAU"\
}

// fixed errors path
#define ERROR_404_PATH "https://img.freepik.com/vecteurs-premium/modele-page-web-erreur-404-chat-mignon_540634-1.jpg"
#define ERROR_403_PATH "https://i.pinimg.com/736x/09/21/da/0921daddd656105719f6abd879428548.jpg"
#define ERROR_418_PATH "http://douceuretdetente.d.o.pic.centerblog.net/f4ef4eb7.jpg"
#define ERROR_500_PATH "https://i.pinimg.com/originals/f0/e8/b7/f0e8b7f3120f06619fa6a2750476d3d6.jpg"


typedef enum e_status
{
	SUCCESS,
	FAILURE
} e_status;

extern int 	g_err_log_fd;
extern bool g_exit;

extern	std::vector<std::string>							g_http_methods;
typedef	std::vector<std::string>::const_iterator			it_method;

int protected_write(int const fd, std::string const &str);
int protected_write_no_endl(int const fd, std::string const &str);

int protected_write_log(std::string const str);
int protected_write_log_no_endl(std::string const str);

#endif