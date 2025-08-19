#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;
    int i;

    while (1)
    {
        printf("#cisfun$ ");
        if (fgets(input, MAX_INPUT, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';  /* Remove newline */

        i = 0;
        token = strtok(input, " \t");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        if (pid == 0)
        {
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(&status);
        }
    }

    return 0;
}
