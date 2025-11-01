/*
 * Command Shell Implementation
 *
 * This file implements an interactive command-line shell with
 * basic file system commands.
 */

#include "shell.h"
#include "uart.h"
#include "string.h"
#include "../filesystem/memfs.h"

/*
 * Command buffer
 */
static char command_buffer[MAX_COMMAND_LEN];

/*
 * Parse command into arguments
 * Returns number of arguments
 */
static int parse_command(char *cmd, char **argv) {
    int argc = 0;
    char *p = cmd;

    /*
     * Skip leading whitespace
     */
    while (*p == ' ' || *p == '\t') {
        p++;
    }

    /*
     * Parse arguments separated by spaces
     */
    while (*p != '\0' && argc < MAX_ARGS) {
        argv[argc++] = p;

        /*
         * Find end of current argument
         */
        while (*p != '\0' && *p != ' ' && *p != '\t') {
            p++;
        }

        /*
         * Null-terminate the argument
         */
        if (*p != '\0') {
            *p = '\0';
            p++;

            // Skip whitespace before next argument
            while (*p == ' ' || *p == '\t') {
                p++;
            }
        }
    }

    return argc;
}

/*
 * Command: help
 * Display available commands
 */
static void cmd_help(int argc, char **argv) {
    (void)argc;
    (void)argv;

    uart_puts("\nAvailable commands:\n");
    uart_puts("  help              - Show this help message\n");
    uart_puts("  clear             - Clear the screen\n");
    uart_puts("  echo <text>       - Print text to console\n");
    uart_puts("  ls                - List all files\n");
    uart_puts("  cat <filename>    - Display file contents\n");
    uart_puts("  edit <file> <txt> - Create/edit a file\n");
    uart_puts("  rm <filename>     - Delete a file\n");
    uart_puts("\n");
}

/*
 * Command: clear
 * Clear the screen using ANSI escape codes
 */
static void cmd_clear(int argc, char **argv) {
    (void)argc;
    (void)argv;

    /*
     * ANSI escape sequence to clear screen and move cursor to top
     */
    uart_puts("\033[2J\033[H");
}

/*
 * Command: echo
 * Print arguments to console
 */
static void cmd_echo(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        uart_puts(argv[i]);
        if (i < argc - 1) {
            uart_putc(' ');
        }
    }
    uart_putc('\n');
}

/*
 * Callback for listing files
 */
static void list_file_callback(const char *name, size_t size) {
    uart_puts("  ");
    uart_puts(name);
    uart_puts(" (");

    // Convert size to string and print
    char size_str[32];
    int pos = 0;
    size_t temp = size;

    if (temp == 0) {
        size_str[pos++] = '0';
    } else {
        // Convert number to string (reverse order)
        int start = pos;
        while (temp > 0) {
            size_str[pos++] = '0' + (temp % 10);
            temp /= 10;
        }
        // Reverse the digits
        for (int i = start, j = pos - 1; i < j; i++, j--) {
            char c = size_str[i];
            size_str[i] = size_str[j];
            size_str[j] = c;
        }
    }

    size_str[pos] = '\0';
    uart_puts(size_str);
    uart_puts(" bytes)\n");
}

/*
 * Command: ls
 * List all files
 */
static void cmd_ls(int argc, char **argv) {
    (void)argc;
    (void)argv;

    int count = fs_get_file_count();

    if (count == 0) {
        uart_puts("No files.\n");
    } else {
        uart_puts("Files:\n");
        fs_list_files(list_file_callback);
    }
}

/*
 * Command: cat
 * Display file contents
 */
static void cmd_cat(int argc, char **argv) {
    if (argc < 2) {
        uart_puts("Usage: cat <filename>\n");
        return;
    }

    const char *content = fs_read_file(argv[1]);

    if (content == NULL) {
        uart_puts("Error: File '");
        uart_puts(argv[1]);
        uart_puts("' not found.\n");
        return;
    }

    uart_puts(content);
    uart_putc('\n');
}

/*
 * Command: edit
 * Create or edit a file
 */
static void cmd_edit(int argc, char **argv) {
    if (argc < 3) {
        uart_puts("Usage: edit <filename> <content>\n");
        return;
    }

    /*
     * Combine all arguments after filename into content
     * This allows content with spaces
     */
    char content[MAX_COMMAND_LEN];
    content[0] = '\0';

    for (int i = 2; i < argc; i++) {
        strcat(content, argv[i]);
        if (i < argc - 1) {
            strcat(content, " ");
        }
    }

    /*
     * Write the file
     */
    if (fs_write_file(argv[1], content) == 0) {
        uart_puts("File '");
        uart_puts(argv[1]);
        uart_puts("' saved.\n");
    } else {
        uart_puts("Error: Could not save file.\n");
    }
}

/*
 * Command: rm
 * Delete a file
 */
static void cmd_rm(int argc, char **argv) {
    if (argc < 2) {
        uart_puts("Usage: rm <filename>\n");
        return;
    }

    if (fs_delete_file(argv[1]) == 0) {
        uart_puts("File '");
        uart_puts(argv[1]);
        uart_puts("' deleted.\n");
    } else {
        uart_puts("Error: File '");
        uart_puts(argv[1]);
        uart_puts("' not found.\n");
    }
}

/*
 * Execute a command
 */
static void execute_command(char *cmd) {
    char *argv[MAX_ARGS];
    int argc = parse_command(cmd, argv);

    if (argc == 0) {
        return;  // Empty command
    }

    /*
     * Dispatch to appropriate command handler
     */
    if (strcmp(argv[0], "help") == 0) {
        cmd_help(argc, argv);
    } else if (strcmp(argv[0], "clear") == 0) {
        cmd_clear(argc, argv);
    } else if (strcmp(argv[0], "echo") == 0) {
        cmd_echo(argc, argv);
    } else if (strcmp(argv[0], "ls") == 0) {
        cmd_ls(argc, argv);
    } else if (strcmp(argv[0], "cat") == 0) {
        cmd_cat(argc, argv);
    } else if (strcmp(argv[0], "edit") == 0) {
        cmd_edit(argc, argv);
    } else if (strcmp(argv[0], "rm") == 0) {
        cmd_rm(argc, argv);
    } else {
        uart_puts("Unknown command: ");
        uart_puts(argv[0]);
        uart_puts("\nType 'help' for available commands.\n");
    }
}

/*
 * Main shell loop
 */
void shell_run(void) {
    uart_puts("\n");
    uart_puts("========================================\n");
    uart_puts("       Welcome to MyOS Shell!          \n");
    uart_puts("========================================\n");
    uart_puts("\n");
    uart_puts("Type 'help' for available commands.\n");
    uart_puts("\n");

    /*
     * Main command loop
     */
    while (1) {
        uart_puts("myos> ");

        /*
         * Read command from user
         */
        uart_gets(command_buffer, MAX_COMMAND_LEN);

        /*
         * Execute the command
         */
        execute_command(command_buffer);
    }
}
