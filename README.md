# Simple Shell (hsh)

## What is this?

This is a small program called a shell.  
It lets you run commands like `/bin/ls` or `ls`.  
It works like a simple version of the Unix shell.

---

## What can it do?

- Show a prompt to type commands.  
- Run commands with arguments.  
- Find commands in the PATH environment.  
- Run commands in a new process.  
- You can type `exit` to close the shell.

---

## Files

- `main.c` — main program and input loop.  
- `execute.c` — runs the commands.  
- `path.c` — finds commands in the PATH.  
- `main.h` — header file with declarations.

---

## How to compile?

Use this command to make the program:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
