#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_ARGS 64
#define MAX_LINE 1024

extern char **environ;

/**
 * get_path_env - Gets the PATH environment variable without getenv
 * Return: pointer to PATH string, or NULL if not found
 */
char *get_path_env(void)
{
    int i;
    char *path_prefix = "PATH=";

    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(environ[i], path_prefix, 5) == 0)
            return environ[i] + 5;
    }
    return NULL;
}

/**
 * find_command_in_path - Finds the full path of a command in PATH dirs
 * @command: command to find
 * Return: full path string (malloced) or NULL if not found
 */
char *find_command_in_path(char *command)
{
    char *path_env;
    char *path_env_dup;
    char *token;
    char *full_path;
    size_t len;
    int found;
    char *dir;

    found = 0;
    path_env = get_path_env();
    if (path_env == NULL)
        return NULL;

    path_env_dup = strdup(path_env);
    if (path_env_dup == NULL)
        return NULL;

    token = strtok(path_env_dup, ":");
    while (token != NULL)
    {
        len = strlen(token) + strlen(command) + 2; /* '/' + '\0' */
        full_path = malloc(len);
        if (full_path == NULL)
        {
            free(path_env_dup);
            return NULL;
        }

        strcpy(full_path, token);
        strcat(full_path, "/");
        strcat(full_path, command);

        if (access(full_path, X_OK) == 0)
        {
            found = 1;
            free(path_env_dup);
            return full_path; /* caller must free */
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_env_dup);
    return NULL;
}

/**
 * main - Simple shell main loop
 * Return: 0 on success
 */
int main(void)
{
    char line[MAX_LINE];
    char *argv[MAX_ARGS];
    char *token;
    char *cmd_path;
    pid_t pid;
    int status;
    int i;

    while (1)
    {
        /* Print prompt */
        write(STDOUT_FILENO, "#cisfun$ ", 8);

        /* Read input */
        if (fgets(line, MAX_LINE, stdin) == NULL)
            break;

        /* Remove newline */
        line[strcspn(line, "\n")] = '\0';

        /* Tokenize input */
        i = 0;
        token = strtok(line, " ");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            argv[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        /* Empty input, continue */
        if (argv[0] == NULL)
            continue;

        /* Built-in exit */
        if (strcmp(argv[0], "exit") == 0)
            break;

        /* Find command in PATH or check if absolute/path */
        if (argv[0][0] == '/' || argv[0][0] == '.')
        {
            cmd_path = argv[0];
        }
        else
        {
            cmd_path = find_command_in_path(argv[0]);
            if (cmd_path == NULL)
            {
                write(STDERR_FILENO, argv[0], strlen(argv[0]));
                write(STDERR_FILENO, ": command not found\n", 20);
                continue;
            }
        }

        /* Fork and exec */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            if (cmd_path != argv[0])
                free(cmd_path);
            continue;
        }

        if (pid == 0)
        {
            /* Child process */
            execve(cmd_path, argv, environ);
            /* If execve fails */
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent waits */
            wait(&status);
            if (cmd_path != argv[0])
                free(cmd_path);
        }
    }

    return 0;
}
