#include "shell.h"

/**
 * read_line - Read a line from stdin
 * Return: pointer to the line
 */
char *read_line(void)
{
	char *line = NULL;
	size_t size = 0;

	if (getline(&line, &size, stdin) == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * split_line - Tokenize the input line
 * @line: the input string
 * Return: NULL-terminated array of strings
 */
char **split_line(char *line)
{
	char *token = NULL, **tokens = malloc(64 * sizeof(char *));
	int i = 0;

	if (!tokens)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token)
	{
		tokens[i++] = token;
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;
	return (tokens);
}
