#include "shell.h"

/**
 * find_path - Looks for command in PATH
 * @cmd: command name
 * Return: full path or NULL
 */
char *find_path(char *cmd)
{
	char *path_env = getenv("PATH");
	char *token;
	static char buffer[1024];

	if (!path_env)
		return (NULL);

	path_env = strdup(path_env); /* avoid strtok modifying env var */
	token = strtok(path_env, ":");
	while (token)
	{
		snprintf(buffer, sizeof(buffer), "%s/%s", token, cmd);
		if (access(buffer, X_OK) == 0)
		{
			free(path_env);
			return (buffer);
		}
		token = strtok(NULL, ":");
	}
	free(path_env);
	return (NULL);
}
