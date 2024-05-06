#include "shared.hpp"
#include <cctype>

uint64_t ft_atoi(std::string nb, e_status &error)
{
	uint64_t res = 0;
	for (int i = 0; i < nb.size(); i++)
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