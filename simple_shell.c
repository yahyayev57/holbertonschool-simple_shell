#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define MAX_ARGS 64
#define MAX_LINE 1024

/**
 * _strdup - duplicates a string
 */
char *_strdup(const char *str)
{
    char *dup;
    int len, i;

    if (!str)
        return NULL;

    len = 0;
    while (str[len] != '\0')
        len++;

    dup = malloc(len + 1);
    if (!dup)
        return NULL;

    for (i = 0; i < len; i++)
        dup[i] = str[i];
    dup[len] = '\0';

    return dup;
}

/**
 * find_command_in_path - search command in PATH env var, returns full path or NULL
 */
char *find_command_in_path(char *command)
{
    char *path_env, *path_env_dup, *token;
    char *full_path;
    int len_dir, len_cmd;

    if (command == NULL)
        return NULL;

    if (command[0] == '/' || command[0] == '.')
    {
        if (access(command, X_OK) == 0)
            return _strdup(command);
        else
            return NULL;
    }

    path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    path_env_dup = _strdup(path_env);
    if (!path_env_dup)
        return NULL;

    token = strtok(path_env_dup, ":");
    while (token != NULL)
    {
        len_dir = strlen(token);
        len_cmd = strlen(command);
        full_path = malloc(len_dir + 1 + len_cmd + 1);
        if (!full_path)
        {
            free(path_env_dup);
            return NULL;
        }

        strcpy(full_path, token);
        full_path[len_dir] = '/';
        strcpy(full_path + len_dir + 1, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_env_dup);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_env_dup);
    return NULL;
}

/**
 * parse_command - splits line into argv array
 */
void parse_command(char *line, char **argv)
{
    int i = 0;
    char *token;

    token = strtok(line, " \t");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        argv[i] = token;
        i++;
        token = strtok(NULL, " \t");
    }
    argv[i] = NULL;
}

/**
 * main - simple shell main loop
 */
int main(void)
{
    char line[MAX_LINE];
    ssize_t bytes_read;
    char *argv[MAX_ARGS];
    char *cmd_path;
    pid_t pid;
    int status;
    int interactive;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            write(STDOUT_FILENO, "$ ", 2);

        bytes_read = read(STDIN_FILENO, line, MAX_LINE - 1);
        if (bytes_read <= 0)
        {
            if (interactive)
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        line[bytes_read] = '\0';

        /* Remove trailing newline */
        if (line[bytes_read - 1] == '\n')
            line[bytes_read - 1] = '\0';

        if (line[0] == '\0')
            continue;

        parse_command(line, argv);
        if (argv[0] == NULL)
            continue;

        cmd_path = find_command_in_path(argv[0]);
        if (cmd_path == NULL)
        {
            write(STDERR_FILENO, argv[0], strlen(argv[0]));
            write(STDERR_FILENO, ": command not found\n", 20);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(cmd_path);
            continue;
        }

        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            free(cmd_path);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
            free(cmd_path);
        }
    }
    return 0;
}
