/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 10:11:31 by yusudemi          #+#    #+#             */
/*   Updated: 2025/01/09 11:59:38 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "ft_printf/ft_printf.h"

volatile sig_atomic_t	g_ack = 0;

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

void	ack(int sig)
{
	(void)sig;
	g_ack = 1;
}

void	send_message(int server_pid, char c)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		if (c & (1 << i))
		{
			if (kill(server_pid, SIGUSR1) == -1)
				exit(EXIT_FAILURE);
		}
		else
		{
			if (kill(server_pid, SIGUSR2) == -1)
				exit(EXIT_FAILURE);
		}
		i++;
		while (!g_ack)
			pause();
		g_ack = 0;
	}
}

int	main(int argc, char *argv[])
{
	int					server_pid;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_printf("Usage: %s <server_pid> <string>\n", argv[0]);
		return (1);
	}
	sa.sa_flags = 0;
	sa.sa_handler = ack;
	sigemptyset(&sa.sa_mask);
	server_pid = ft_atoi(argv[1]);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		exit(EXIT_FAILURE);
	if (server_pid <= 0)
		exit(EXIT_FAILURE);
	while (*argv[2])
	{
		send_message(server_pid, *argv[2]);
		argv[2]++;
	}
	return (0);
}
