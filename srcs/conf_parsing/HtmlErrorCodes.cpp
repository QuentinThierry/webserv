#include "ConfParser.hpp"

bool	does_http_code_exist(std::string code)
{
	std::string const http_error_codes[] =
	{
		"100", "101", "102", "103",
		"200", "201", "202", "203", "204", "205", "206", "207", "208", "226",
		"300", "301", "302", "303", "304", "305", "306", "307", "308",
		"400", "401", "402", "403", "404", "405", "406", "407", "408", "409",
		"410", "411", "412", "413", "414", "415", "416", "417", "418",
		"421", "422", "423", "424", "425", "426", "428", "429", "431", "451",
		"500", "501", "502", "503", "504", "505", "506", "507", "508", "510", "511",
		""
	};
	for (unsigned int i = 0; !http_error_codes[i].empty(); i++) {
		if (http_error_codes[i] == code)
			return true;
	}
	return false;
}

t_http_code	str_to_http_code(std::string code)
{
	if (!does_http_code_exist(code))
		ThrowMisc("unknown http code `" + code + "`");
	return (code[0] - '0') * 100 + (code[1] - '0') * 10 + (code[2] - '0');
}