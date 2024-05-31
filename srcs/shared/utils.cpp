#include "utils.hpp"

uint64_t ft_atoi(std::string nb, e_status &error)
{
	uint64_t res = 0;
	for (unsigned int i = 0; i < nb.size(); i++)
	{
		if (std::isdigit(nb[i]) == 0)
		{
			error = FAILURE;
			return 0;
		}
		if (res > (UINT64_MAX - (nb[i] - '0')) / 10)
		{
			error = FAILURE;
			return 0;
		}
		res = res * 10 + (nb[i] - '0');
	}
	return res;
}

std::string ft_itoa( uint64_t number )
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

bool	is_accessible_directory(char const *name)
{
	int fd = open(name, O_DIRECTORY);
	if (fd == -1)
		return false;
	close(fd);
	return (true);
}

uint64_t	str_to_hex(std::string str, bool &error)
{
	uint64_t res = 0;

	error = false;
	if (str.empty())
		return error = true;
	if (str.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
		return error = true;
	
	for (unsigned int i = 0; i < str.size(); i++)
	{
		res *= 16;
		if (str[i] >= '0' && str[i] <= '9')
			res += str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			res += str[i] - 'a' + 10;
		else if (str[i] >= 'A' && str[i] <= 'F')
			res += str[i] - 'A' + 10;
	}
	return res;
}

std::string	hex_to_str(uint64_t val)
{
	std::string res = "";

	if (val > 15)
		res = hex_to_str(val / 16);
	res += "0123456789abcdef"[val % 16];
	return res;
}
