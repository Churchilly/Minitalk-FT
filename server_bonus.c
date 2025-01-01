#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "minitalk_bonus.h"
#include "ft_printf/ft_printf.h"

t_list	*g_buffer = NULL;

void	server_error(void)
{
	write(1, "Encountered with an error.", 27);
	clear_list(&g_buffer);
	exit(EXIT_FAILURE);
}

void	handler(int sig, siginfo_t *info, void *ucontext)
{
	static int	counter = 0;
	static char current_char = 0;
	t_list			*tmp;

	(void)ucontext;
	if (sig == SIGUSR1)
		current_char |= (1 << counter);
	counter++;
	if (counter == 8)
	{
		tmp = list_new(current_char);
		list_add_back(&g_buffer, tmp);
		if (current_char == '\0')
		{
			if (kill(info->si_pid, SIGUSR1) == -1)
				server_error();
			write_list(g_buffer);
			clear_list(&g_buffer);
		}
		current_char = 0;
		counter = 0;
	}
}

int main(void)
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
