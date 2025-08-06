#include "shell.h"

/**
 * Shells go through three stages of execution:
 * 1. Initialization: read and execute configuration files
 * 2. Interpretation: read and execute commands from the user
 * 3. Termination: clean up and exit
 */


// Main shell function
int main(int argc, char *argv[]) {
    // 1. Initialization

    // 2. Interpretation
    lsh_loop(); // Start the shell loop to read and execute commands

    // 3. Termination

    return EXIT_SUCCESS;
}