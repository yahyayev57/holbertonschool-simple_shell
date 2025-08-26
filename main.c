#include "shell.h"

/**
 * main - Entry point
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 always
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	char **args = NULL;
	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		line = read_line();
		if (!line)
			break;

		args = split_line(line);
		if (!args || !args[0])
		{
			free(line);
			free(args);
			continue;
		}

		execute(args, argv[0]); /* fixed: passing char * */

		free(line);
		free(args);
	}

	return (0);
}
