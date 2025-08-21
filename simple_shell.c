/* simple_shell.c */

#include "shell.h"

/**
 * main - main loop of the simple shell
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL, *cmd = NULL;
	size_t len = 0;
	ssize_t nread;
	char *argv[64];
	pid_t pid;
	int i;

	while (1)
	{
		printf(":) ");
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		line[strcspn(line, "\n")] = '\0';

		tokenize_input(line, argv);
		if (!argv[0])
			continue;

		cmd = resolve_path(argv[0]);
		if (!cmd)
		{
			fprintf(stderr, "%s: command not found\n", argv[0]);
			continue;
		}

		pid = fork();
		if (pid == 0)
		{
			execve(cmd, argv, environ);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else if (pid > 0)
		{
			wait(NULL);
			free(cmd);
		}
		else
		{
			perror("fork");
			free(cmd);
		}
	}

	free(line);
	return (0);
}
