#include "shell.h"

/**
 * execute - Handle command execution with fork + execve
 * @args: argument list
 * @argv: program name for error messages
 */
void execute(char **args, char *argv)
{
	pid_t pid;
	char *cmd = args[0], *path = NULL;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			path = cmd;
	}
	else
	{
		path = find_path(cmd);
	}

	if (!path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", argv[0], cmd);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
			perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		wait(NULL);
}
