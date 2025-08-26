#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **tokenize(char *line)
{
    char **tokens = malloc(64 * sizeof(char *));
    char *token;
    int i = 0;

    if (!tokens)
        return NULL;

    token = strtok(line, " ");
    while (token)
    {
        tokens[i++] = token;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return tokens;
}
