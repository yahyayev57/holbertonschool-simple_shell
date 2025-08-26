#include "shell.h"

/**
 * find_path - Search command in PATH
 * @cmd: command name
 * Return: full path if found, else NULL
 */
char *find_path(char *cmd)
{
	char *path = getenv("PATH");
	char *token = NULL, *full = malloc(1024);

	if (!path || !full)
		return (NULL);

	token = strtok(path, ":");
	while (token)
	{
		snprintf(full, 1024, "%s/%s", token, cmd);
		if (access(full, X_OK) == 0)
			return (full);
		token = strtok(NULL, ":");
	}
	free(full);
	return (NULL);
}
