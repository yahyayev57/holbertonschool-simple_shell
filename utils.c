#include "shell.h"

/**
 * read_line - Reads a line from stdin
 * Return: pointer to the line
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;

	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * split_line - Tokenize the input line
 * @line: input string
 * Return: array of strings
 */
char **split_line(char *line)
{
	char *token = NULL;
	char **tokens = malloc(64 * sizeof(char *));
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
