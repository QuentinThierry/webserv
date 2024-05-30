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

e_status empty_sstream_in_string(std::string &dest, std::stringstream &stream_to_empty)
{	
	std::string body_content;
	while (!stream_to_empty.eof() && stream_to_empty.peek() != EOF)
	{
		if (!std::getline(stream_to_empty, body_content))
			return (FAILURE);
		dest += body_content;
		if (!stream_to_empty.eof())
			dest += "\n";
	}
	return (SUCCESS);
}