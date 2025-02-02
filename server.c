/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 10:11:50 by yusudemi          #+#    #+#             */
/*   Updated: 2025/01/09 11:58:31 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "ft_printf/ft_printf.h"

void	server_error(void)
{
	write(1, "Encountered with an error.", 27);
	exit(EXIT_FAILURE);
}

void	handler(int sig, siginfo_t *info, void *ucontext)
{
	static int	counter = 0;
	static char	current_char = 0;

	(void)ucontext;
	if (sig == SIGUSR1)
		current_char |= (1 << counter);
	counter++;
	if (counter == 8)
	{
		write(1, &current_char, 1);
		current_char = 0;
		counter = 0;
	}
	if (kill(info->si_pid, SIGUSR1) == -1)
		server_error();
}

int	main(void)
{
	struct sigaction	sa;

	ft_printf("Server PID: %d\n", getpid());
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		server_error();
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		server_error();
	while (42)
		pause();
	return (EXIT_SUCCESS);
}
