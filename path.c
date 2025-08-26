#include "main.h"

extern char **environ;

char *find_path(char *command)
{
    char *path = getenv("PATH");
    char *dir;
    char full_path[1024];

    if (access(command, X_OK) == 0)
        return strdup(command);

    dir = strtok(path, ":");
    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
            return strdup(full_path);

        dir = strtok(NULL, ":");
    }

    return NULL;
}
