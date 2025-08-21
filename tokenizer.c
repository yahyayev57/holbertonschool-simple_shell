#include "shell.h"

/**
 * tokenize_input - Splits a line into arguments
 * @line: Input line
 * @argv: Array to fill with arguments
 */
void tokenize_input(char *line, char **argv)
{
	int i = 0;

	argv[i] = strtok(line, " ");
	while (argv[i])
	{
		i++;
		argv[i] = strtok(NULL, " ");
	}
}
