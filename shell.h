// Standard C library headers
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// Define constants for the shell program
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

// function declarations for the shell program
void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_splint_line(char *line);
int lsh_execute(char **args);