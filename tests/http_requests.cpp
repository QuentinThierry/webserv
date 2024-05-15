# include "HttpRequestDelete.class.hpp"
# include "HttpRequestPost.class.hpp"
# include "HttpRequestGet.class.hpp"

static void check_field();

int g_err_log_fd = -1;

int main()
{
	check_field();
	return 0;
}

static void check_field()
{
	std::cout << "=== check_field ===" << std::endl;

	std::vector<std::string>  fields_str_ok;
	std::vector<std::string>  fields_str_ko;

	fields_str_ko.push_back("no_colon_ko");

	fields_str_ok.push_back("");
	fields_str_ok.push_back("name_ok: ok");
	fields_str_ok.push_back("name_ok_ABC: ok");
	fields_str_ok.push_back("name_ok-special: ok");
	fields_str_ok.push_back("name_ok_special: ok");
	fields_str_ok.push_back("name_ok1digit: ok");

	fields_str_ko.push_back("name_ko.char: ok");
	fields_str_ko.push_back("name_ko_space : ok");
	fields_str_ko.push_back("name_ko space: ok");
	fields_str_ko.push_back("name_ko\tspace: ok");
	fields_str_ko.push_back("name_ko\nreturn: ok");

	fields_str_ok.push_back("name_ok: this_is_ok");
	fields_str_ok.push_back("name_ok: this_is;ok");
	fields_str_ok.push_back("name_ok: this_is_8_ok");
	fields_str_ok.push_back("name_ok: simple_ok, with_next");
	fields_str_ok.push_back("name_ok:   simple_ok_space  ");
	fields_str_ok.push_back("name_ok: simple_ok;q=1");
	fields_str_ok.push_back("name_ok: \"quoted_ok\"abc");
	fields_str_ok.push_back("name_ok: \"quoted_ok\"continued");
	fields_str_ok.push_back("name_ok: \"quoted_ok\", next");

	fields_str_ko.push_back("name_ok: ko space");
	fields_str_ko.push_back("name_ok: ko, forbidden\ttab ");
	fields_str_ko.push_back("name_ok: ko, forbidden\vverticaltab ");

	fields_str_ok.push_back("multiple_fields: value1\r\nsecond_one: value2");

	std::cout << "Fields ok:" << std::endl;
	for (size_t	i = 0; i < fields_str_ok.size(); ++i)
	{
		std::string request = "GET / HTTP/1.1\r\n";
		request += fields_str_ok[i];
		try {
			HttpRequestGet test(request);
			std::cout << " test " << i << ":\tok" << std::endl;
		} catch (std::exception & e) {
			std::cout << " test " << i << ":\tko (\"" << fields_str_ok[i] << "\")" << std::endl;
			
		}
	}

	std::cout << "Fields ko:" << std::endl;
	for (size_t	i = 0; i < fields_str_ko.size(); ++i)
	{
		std::string request = "GET / HTTP/1.1\r\n";
		request += fields_str_ko[i];
		try {
			HttpRequestGet test(request);
			std::cout << " test " << i << ":\tko (\"" << fields_str_ko[i] << "\")" << std::endl;
		} catch (ExceptionHttpStatusCode & e) {
			std::cout << " test " << i << ":\tok" << std::endl;
		} catch (std::exception & e){
			std::cout << " test " << i << ":\tko (\"" << fields_str_ko[i] << "\")" << std::endl;
		}
	}
}




static void check_request_line()
{
	//TODO
}