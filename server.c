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
	ft_printf("Server PID: %d\n", getpid()); // replace with urs
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (42)
		pause();
	return (0);
}
