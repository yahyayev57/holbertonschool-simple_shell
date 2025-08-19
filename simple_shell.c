#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/**
 * main - Simple shell 0.1 entry point
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    char *args[2];

    while (1)
    {
        printf("#cisfun$ ");
        read = getline(&line, &len, stdin);
        if (read == -1) /* Handle EOF (Ctrl+D) */
        {
            printf("\n");
            break;
        }

        if (line[read - 1] == '\n')
            line[read - 1] = '\0'; /* Remove trailing newline */

        if (line[0] == '\0') /* Empty input */
            continue;

        args[0] = line;
        args[1] = NULL;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(line);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            /* Child process */
            if (execve(args[0], args, environ) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Parent process waits */
            wait(NULL);
        }
    }

    free(line);
    return (0);
}
