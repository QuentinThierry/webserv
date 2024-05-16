/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protected_write.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:15:41 by acardona          #+#    #+#             */
/*   Updated: 2024/05/16 14:02:28 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shared.hpp"
#include "util.hpp"

int protected_write(int const fd, std::string const str)
{
	int	n = 0;

	if (fd >= 0)
	{
		n = write(fd, str.c_str(), str.size());
		write(fd, "\n", 1);
	}
	return (n);
}

std::string error_message_server(Server const &server, std::string const &message)
{
	return message + " " + server.getHost() + ":" + ft_itoa(server.getPort()) + "\n";
}
