#include "shell.h"

void lsh_loop(void) {
    /**
     * Read: Read the command from the standard input
     * Parse: Seperate the command into its components
     * Execute: Execute the parsed command
     */
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = lsh_read_line(); // Read
        args = lsh_splint_line(line); // Parse
        status = lsh_execute(args); // Execute
    } while (status); // Easy way to keep the shell running and exiting when needed
}

/**
 * Allocates memory for the buffer, then reads a line from the standard input.
 * If the input exceeds the buffer size, it reallocates more memory.
 * Returns a pointer to the buffer containing the input.
 */
char *lsh_read_line(void) { // Implementation of getline() in the C standard library
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    // Check for memory allocation failure
    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0'; // Null-terminate the buffer
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // Edge case: if we exceed the buffer size
        if (position >= bufsize) {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/**
 * Splits the input line into tokens based on deliminators in shell.h.
 * If the input exceeds the buffer size, it reallocates more memory.
 * Returns an array of strings representing the tokens.
 */
char **lsh_split_line(char *line){
    int bufsize = LSH_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    // Check memory allocation
    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL){
        tokens[position] = token;
        position++;

        // Reallocate memory if necessary
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM); // Get the next token
    }

    tokens[position] = NULL; // Null-terminate the array
    return tokens;
}