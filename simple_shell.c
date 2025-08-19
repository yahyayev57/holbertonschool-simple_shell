#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;

    while (1)
    {
        printf("#cisfun$ ");
        if (!fgets(input, MAX_INPUT, stdin))
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';  // Remove newline

        int i = 0;
        token = strtok(input, " \t");
        while (token && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            wait(&status);
        }
        else
        {
            perror("fork");
        }
    }
    return 0;
}
