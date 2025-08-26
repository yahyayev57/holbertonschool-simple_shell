#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

char **tokenize(char *line);
char *resolve_path(char *command);

int main(void)
{
    char *line = NULL, *cmd = NULL;
    size_t len = 0;
    ssize_t nread;
    char **argv;
    pid_t pid;
    int status;

    while (1)
    {
        write(1, ":) ", 3);
        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        line[nread - 1] = '\0';  // Remove newline
        argv = tokenize(line);
        if (!argv || !argv[0])
            continue;

        cmd = resolve_path(argv[0]);
        if (!cmd)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            execve(cmd, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
        }

        free(argv);
    }

    free(line);
    return 0;
}
