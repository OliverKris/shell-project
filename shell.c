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
        args = lsh_split_line(line); // Parse
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

/**
 * Allows our shell to execute programs by forking a new process.
 * It uses execvp to run the command with arguments parsed from the input.
 * Returns 1 on success, or 0 if the command is "exit".
 */
int lsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE); // Exit child process if exec fails
    } else if (pid < 0) {
        // Forking error
        perror("lsh");
        
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// Contains a list of builtin commands
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

// Command to map the builtin commands to their respective functions
int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/**
 * Parses the incoming command and checks if it is a built-in command.
 * If it is, it calls the corresponding function.
 * If not, it calls lsh_launch to execute the command.
 */
int lsh_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        return 1; // No command entered
    }

    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0){
            // Call the corresponding builtin function
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args); // If not a builtin command, launch it
}

/**
 * Builting shell function implementations
 */

int lsh_cd(char **args) {
    if (args[1] == NULL) { // No argument provided
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0){ // Use chdir to change directory
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args) {
    printf("Oliver Krisetya's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built-in commands:\n");

    // Loop through the built-in commands and print them
    for(int i = 0; i < lsh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char **args) {
    return 0; // Return 0 to indicate exit
}