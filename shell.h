#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

extern char **environ;

char *read_line(void);
char **split_line(char *line);
char *find_path(char *cmd);
void execute(char **args, char *program_name);

#endif
