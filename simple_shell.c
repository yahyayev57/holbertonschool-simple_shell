#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 10
extern char **environ;

char *trim_spaces(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    *(end + 1) = '\0';

    return str;
}

int split_line(char *line, char **args)
{
    int i = 0;
    char *token;

    token = strtok(line, " \t\r\n");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;
    return i;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    char *args[MAX_ARGS];
    int argc;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        char *cmd = trim_spaces(line);
        if (cmd[0] == '\0')
            continue;

        argc = split_line(cmd, args);
        if (argc == 0)
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(line);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (execve(args[0], args, environ) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(NULL);
        }
    }

    free(line);
    return (0);
}
