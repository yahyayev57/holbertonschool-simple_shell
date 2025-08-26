#include "shell.h"

/**
 * execute - Executes a command if valid
 * @args: command args
 * @program_name: program name (argv[0])
 */
void execute(char **args, char *program_name)
{
	pid_t pid;
	char *cmd = args[0];
	char *path = NULL;

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
		fprintf(stderr, "%s: 1: %s: not found\n", program_name, cmd);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
}
