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
	while ((unsigned char)*str >= '0' && (unsigned char)*str <= '9')
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
	int i;

	i = 0;
	while (i < 8)
	{
		if (message & (1 << i))
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		usleep(42);
		i++;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		ft_printf("Usage: %s <server_pid> <string>\n", argv[0]);
		return (1);
	}
	while (*argv[2])
	{
		send_message(ft_atoi(argv[1]), *argv[2]);
		argv[2]++;
	}
	send_message(ft_atoi(argv[1]), '\n');
	return (0);
}