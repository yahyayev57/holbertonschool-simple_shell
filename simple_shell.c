#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_ARGS 64

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	char *argv[MAX_ARGS];
	int i;
	ssize_t read;
	pid_t pid;
	int status;
	char *token;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		i = 0;
		token = strtok(line, " ");
		while (token != NULL && i < MAX_ARGS - 1)
		{
			argv[i++] = token;
			token = strtok(NULL, " ");
		}
		argv[i] = NULL;

		if (argv[0] == NULL)
			continue;

		pid = fork();
		if (pid == 0)
		{
			if (execve(argv[0], argv, NULL) == -1)
				perror("execve");
			exit(EXIT_FAILURE);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
		}
		else
		{
			perror("fork");
		}
	}

	free(line);
	return 0;
}
