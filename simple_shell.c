#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * find_command - search command in PATH
 * @cmd: command to find
 * Return: full path to command or NULL
 */
char *find_command(char *cmd)
{
	char *path = getenv("PATH");
	char *path_copy = NULL, *token = NULL, *full_path = NULL;
	size_t len;

	if (!path)
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token)
	{
		len = strlen(token) + strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		snprintf(full_path, len, "%s/%s", token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path); /* FOUND ✅ */
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL); /* NOT FOUND ❌ */
}

/**
 * main - basic shell loop
 * Return: 0 always
 */
int main(void)
{
	char *line = NULL, *argv[64], *cmd = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int i;

	while (1)
	{
		printf(":) ");
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		line[strcspn(line, "\n")] = '\0'; /* remove \n */

		/* Tokenize line */
		i = 0;
		argv[i] = strtok(line, " ");
		while (argv[i])
		{
			i++;
			argv[i] = strtok(NULL, " ");
		}
		if (!argv[0])
			continue;

		/* Handle PATH or full path */
		if (argv[0][0] == '/' || argv[0][0] == '.')
		{
			if (access(argv[0], X_OK) == 0)
				cmd = strdup(argv[0]);
		}
		else
		{
			cmd = find_command(argv[0]);
		}

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
