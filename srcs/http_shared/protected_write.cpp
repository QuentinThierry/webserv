/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protected_write.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:15:41 by acardona          #+#    #+#             */
/*   Updated: 2024/05/15 21:02:36 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shared.hpp"

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
