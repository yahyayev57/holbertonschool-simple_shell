#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Max args count */
#define MAX_ARGS 64

/**
 * find_command_in_path - Find full path of command in PATH
 * @cmd: Command name (like "ls")
 * Return: full path string or NULL if not found
 */
char *find_command_in_path(char *cmd)
{
    char *path_env, *path_copy, *dir;
    static char full_path[1024];

    if (cmd == NULL)
        return NULL;

    /* If cmd contains '/', treat as path */
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

    /* Check if shell is interactive */
    int interactive = isatty(STDIN_FILENO);

    while (1)
    {
        /* Print prompt if interactive */
        if (interactive)
            write(STDOUT_FILENO, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            /* EOF or error */
            if (interactive)
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* Remove trailing newline */
        line[strcspn(line, "\n")] = '\0';

        /* Skip empty lines */
        if (line[0] == '\0')
            continue;

        /* Parse input into argv */
        i = 0;
        argv[i] = strtok(line, " \t");
        while (argv[i] != NULL && i < MAX_ARGS - 1)
        {
            i++;
            argv[i] = strtok(NULL, " \t");
        }
        argv[i] = NULL;

        /* Find command path */
        char *cmd_path = find_command_in_path(argv[0]);
        if (cmd_path == NULL)
        {
            dprintf(STDERR_FILENO, "%s: command not found\n", argv[0]);
            continue;
        }

        /* Fork and execute */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            /* Child */
            execve(cmd_path, argv, environ);
            /* If execve returns, error */
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent */
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
