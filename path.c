#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char *find_path(char *command)
{
    char *path = getenv("PATH");
    char *dir, *full_path;
    size_t len;

    if (access(command, X_OK) == 0)
        return command;

    dir = strtok(path, ":");
    while (dir)
    {
        len = strlen(dir) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
            return NULL;

        snprintf(full_path, len, "%s/%s", dir, command);

        if (access(full_path, X_OK) == 0)
            return full_path;

        free(full_path);
        dir = strtok(NULL, ":");
    }

    return NULL;
}
