#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define DELIM " \t\r\n\a"

/**
 * main - Entry point for a simple shell.
 * Return: 0 on success.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *token;
	char *argv[BUFFER_SIZE];
	pid_t pid;
	int status;
	int i;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		token = strtok(line, "\n");
		while (token != NULL)
		{
			char *subtoken = strtok(token, DELIM);
			i = 0;

			while (subtoken != NULL && i < BUFFER_SIZE - 1)
			{
				argv[i++] = subtoken;
				subtoken = strtok(NULL, DELIM);
			}
			argv[i] = NULL;

			if (argv[0] != NULL)
			{
				pid = fork();
				if (pid == 0)
				{
					if (execvp(argv[0], argv) == -1)
						perror(argv[0]);
					exit(EXIT_FAILURE);
				}
				else if (pid < 0)
				{
					perror("fork");
				}
				else
				{
					wait(&status);
				}
			}
			token = strtok(NULL, "\n");
		}
	}
	free(line);
	return (0);
}
