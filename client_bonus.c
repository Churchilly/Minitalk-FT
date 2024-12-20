#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t g_ack = 0;

void	ack_handler(int signal)
{
	(void)signal;
	write(1, "message sending acknowledged\n", 30);
	g_ack = 1;
}

void	send_message(int server_pid, int c)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		g_ack = 0;
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
		usleep(42);
		i++;
	}
}

int main(int argc, char *argv[])
{
	int	server_pid;
	struct sigaction sa;

	if (argc != 3)
	{
		printf("Usage: %s <server_pid> <string>\n", argv[0]);
		return 1;
	}
	sa.sa_handler = ack_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		exit(EXIT_FAILURE);
	server_pid = atoi(argv[1]);
	while (*argv[2])
	{
		send_message(server_pid, *argv[2]);
		argv[2]++;
	}
	send_message(server_pid, '\0');
	while (!g_ack)
		pause();
	return (0);
}
