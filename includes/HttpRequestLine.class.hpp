#ifndef HTTPREQUESTLINE_CLASS_HPP
# define HTTPREQUESTLINE_CLASS_HPP

# include "shared.hpp"
# include <vector>
// # include "HttpTools.hpp"
// # include "HttpField.class.hpp"

class HttpRequestLine
{
    public:
        std::vector<std::string> const getFieldValue(std::string const & filed_name,
				e_status &success_status) const
		{
			(void) filed_name;
			std::vector<std::string> test(1, "test");
			success_status = SUCCESS;
			return test;
		};
        HttpRequestLine(){};
    private:

};

#endif