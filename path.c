#include "shell.h"

/**
 * find_path - Looks for command in PATH
 * @cmd: command name
 * Return: full path or NULL
 */
char *find_path(char *cmd)
{
	char *path = getenv("PATH");
	char *token, *full;
	static char buffer[1024];

	if (!path)
		return (NULL);

	token = strtok(path, ":");
	while (token)
	{
		snprintf(buffer, sizeof(buffer), "%s/%s", token, cmd);
		if (access(buffer, X_OK) == 0)
			return (buffer);
		token = strtok(NULL, ":");
	}
	return (NULL);
}
