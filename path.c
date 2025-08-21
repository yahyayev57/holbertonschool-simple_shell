#include "shell.h"

/**
 * resolve_path - Finds the full path of a command using PATH
 * @cmd: Command to search for
 * Return: Full path string if found, else NULL
 */
char *resolve_path(char *cmd)
{
	char *path = NULL, *copy, *token, *full_path;
	int i = 0;
	size_t len;

	/* Find PATH manually in environ */
	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
		i++;
	}
	if (!path)
		return (NULL);

	/* Absolute or relative path case */
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	copy = strdup(path);
	if (!copy)
		return (NULL);

	token = strtok(copy, ":");
	while (token)
	{
		len = strlen(token) + strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(copy);
			return (NULL);
		}
		snprintf(full_path, len, "%s/%s", token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(copy);
	return (NULL);
}
