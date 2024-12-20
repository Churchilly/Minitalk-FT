#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
		printf("Usage: %s <server_pid> <string>\n", argv[0]);
		return (1);
	}
	while (*argv[2])
	{
		send_message(atoi(argv[1]), *argv[2]);
		argv[2]++;
	}
	send_message(atoi(argv[1]), '\n');
	return (0);
}