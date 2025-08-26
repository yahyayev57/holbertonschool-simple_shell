#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

/**
 * find_path - Finds full path of a command by searching in PATH env variable
 * @command: The command to find (e.g. "ls")
 *
 * Return: A malloc'ed string with the full path if found, or NULL if not found
 */
char *find_path(char *command)
{
    char *path_env, *path_env_copy, *dir;
    char full_path[1024];
    int len;

    /* If command contains a '/' try to execute directly */
    if (strchr(command, '/') != NULL)
    {
        if (access(command, X_OK) == 0)
            return strdup(command);
        else
            return NULL;
    }

    /* Get PATH env variable */
    path_env = getenv("PATH");
    if (path_env == NULL)
        return NULL;

    /* Duplicate PATH because strtok modifies the string */
    path_env_copy = strdup(path_env);
    if (path_env_copy == NULL)
        return NULL;

    dir = strtok(path_env_copy, ":");
    while (dir != NULL)
    {
        /* Build full path */
        len = snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (len < 0 || len >= (int)sizeof(full_path))
        {
            dir = strtok(NULL, ":");
            continue;
        }

        /* Check if file is executable */
        if (access(full_path, X_OK) == 0)
        {
            free(path_env_copy);
            return strdup(full_path);
        }

        dir = strtok(NULL, ":");
    }

    free(path_env_copy);
    return NULL;
}
s
