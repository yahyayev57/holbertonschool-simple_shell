#include "shell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * main - Simple shell 0.3
 *
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char **argv;
    char *cmd;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        argv = tokenize(line);
        if (!argv || !argv[0])
        {
            free(argv);
            continue;
        }

        if (strcmp(argv[0], "exit") == 0)
        {
            free(argv);
            break;
        }

        if (strcmp(argv[0], "env") == 0)
        {
            extern char **environ;
            for (int j = 0; environ[j]; j++)
                write(STDOUT_FILENO, environ[j], strlen(environ[j])), write(STDOUT_FILENO, "\n", 1);
            free(argv);
            continue;
        }

        if (argv[0][0] == '/' || argv[0][0] == '.')
        {
            cmd = strdup(argv[0]);
        }
        else
        {
            cmd = resolve_path(argv[0]);
        }

        if (!cmd)
        {
            dprintf(STDERR_FILENO, "%s: command not found\n", argv[0]);
            free(argv);
            continue;
        }

        if (fork() == 0)
        {
            execve(cmd, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            wait(&status);
        }

        free(cmd);
        free(argv);
    }

    free(line);
    return 0;
}
