/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 10:11:31 by yusudemi          #+#    #+#             */
/*   Updated: 2025/01/07 17:37:58 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include "ft_printf/ft_printf.h"

int	ft_atoi(const char *str)
{
	int		sign;
	long	num;

	sign = 1;
	num = 0;
	while (*str == 9 || *str == 10 || *str == 11
		|| *str == 12 || *str == 13 || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		if (num * sign > INT_MAX)
			return (INT_MAX);
		if (num * sign < INT_MIN)
			return (INT_MIN);
		str++;
	}
	return ((int)(num * sign));
}

void	send_message(int server_pid, const char message)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		if (message & (1 << i))
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		usleep(200);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	int	server_pid;
	
	
	if (argc != 3)
	{
		ft_printf("Usage: %s <server_pid> <string>\n", argv[0]);
		return (1);
	}
	server_pid = ft_atoi(argv[1]);
	if (server_pid <= 0)
		exit(EXIT_FAILURE);
	while (*argv[2])
	{
		send_message(server_pid, *argv[2]);
		argv[2]++;
	}
	send_message(server_pid, '\n');
	return (0);
}
