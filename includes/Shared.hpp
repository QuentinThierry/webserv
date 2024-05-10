/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shared.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:07:56 by jvigny            #+#    #+#             */
/*   Updated: 2024/05/09 15:51:31 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_HPP
# define SHARED_HPP

typedef enum e_status
{
	SUCCESS,
	FAILURE
} e_status;

# include <string>
# include <iostream>
# include <exception>
# include <unistd.h>
# include <stdint.h>

extern int const g_err_log_fd;

int protected_write(int const fd, std::string const str);

#endif