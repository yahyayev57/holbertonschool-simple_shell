#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;  /* Add this declaration */

#define MAX_ARGS 64

char *find_command_in_path(char *cmd)
{
    char *path_env, *path_copy, *dir;
    static char full_path[1024];

    if (cmd == NULL)
        return NULL;

    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return cmd;
        else
            return NULL;
    }

    path_env = getenv("PATH");
    if (path_env == NULL)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *argv[MAX_ARGS];
    int i;
    pid_t pid;
    int status;
    int interactive;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            write(STDOUT_FILENO, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            if (interactive)
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0')
            continue;

        i = 0;
        argv[i] = strtok(line, " \t");
        while (argv[i] != NULL && i < MAX_ARGS - 1)
        {
            i++;
            argv[i] = strtok(NULL, " \t");
        }
        argv[i] = NULL;

        /* Declare variables at top */
        char *cmd_path;

        cmd_path = find_command_in_path(argv[0]);
        if (cmd_path == NULL)
        {
            dprintf(STDERR_FILENO, "%s: command not found\n", argv[0]);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
