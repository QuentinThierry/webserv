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
# include <vector>

extern int const	g_err_log_fd;
extern bool 		g_exit;

extern	std::vector<std::string>							g_http_methods;
typedef	std::vector<std::string>::const_iterator			it_method;

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

int protected_write(int const fd, std::string const str);

class ExceptionCltrC: public std::exception
{
	public:
		char const *what() const throw() {return "Interrupt signal received";}
};

#endif