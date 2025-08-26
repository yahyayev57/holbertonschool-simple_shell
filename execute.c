#include "main.h"
#include <sys/wait.h>

extern char **environ;

void execute_command(char **args)
{
    pid_t pid;
    char *full_path;

    pid = fork();

    if (pid == 0)
    {
        full_path = find_path(args[0]);
        if (full_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(1);
        }
        execve(full_path, args, environ);
        perror("execve");
        exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork");
    }
}
