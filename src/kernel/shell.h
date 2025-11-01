/*
 * Command Shell Header
 *
 * Interactive command-line interface for the OS
 */

#ifndef SHELL_H
#define SHELL_H

/*
 * Maximum command line length
 */
#define MAX_COMMAND_LEN 256

/*
 * Maximum number of command arguments
 */
#define MAX_ARGS 16

/*
 * Initialize and start the shell
 * This function does not return
 */
void shell_run(void);

#endif // SHELL_H
