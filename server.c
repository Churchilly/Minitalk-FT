/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 10:11:50 by yusudemi          #+#    #+#             */
/*   Updated: 2025/01/07 17:37:55 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_printf/ft_printf.h"

void	signal_handler(int signal)
{
	static unsigned int	counter = 0;
	static char			current_char = 0;

	if (signal == SIGUSR1)
		current_char |= (1 << counter);
	counter++;
	if (counter == 8)
	{
		write(1, &current_char, 1);
		current_char = 0;
		counter = 0;
	}
}

int	main(void)
{
	ft_printf("Server PID: %d\n", getpid());
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (42)
		pause();
	return (0);
}
