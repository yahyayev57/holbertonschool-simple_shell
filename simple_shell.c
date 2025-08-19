#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

/**
 * main - simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char cmd[MAX_CMD_LEN];
	char *argv[MAX_ARGS];
	pid_t pid;
	int status, i;

	while (1)
	{
		write(STDOUT_FILENO, "#cisfun$ ", 9);

		if (fgets(cmd, sizeof(cmd), stdin) == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove newline character */
		cmd[strcspn(cmd, "\n")] = '\0';

		/* Tokenize command */
		i = 0;
		argv[i] = strtok(cmd, " ");
		while (argv[i] != NULL && i < MAX_ARGS - 1)
		{
			i++;
			argv[i] = strtok(NULL, " ");
		}
		argv[i] = NULL;

		/* Skip empty input */
		if (argv[0] == NULL)
			continue;

		pid = fork();
		if (pid == 0)
		{
			/* Child process */
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else if (pid > 0)
		{
			/* Parent process */
			wait(&status);
		}
		else
		{
			perror("fork");
		}
	}

	return (0);
}
