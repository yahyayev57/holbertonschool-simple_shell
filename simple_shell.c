#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 64

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1)
    {
        printf("#cisfun$ ");
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            perror("getline");
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        char *argv[MAX_ARGS];
        int i = 0;
        char *token = strtok(line, " ");

        while (token != NULL && i < MAX_ARGS - 1)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
            continue;

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        else if (pid == 0)
        {
            execve(argv[0], argv, NULL);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            wait(&status);
        }
    }

    free(line);
    return 0;
}
