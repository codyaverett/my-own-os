/*
 * Kernel Main Entry Point
 *
 * This is the C entry point for our operating system.
 * The bootloader (boot.S) calls this function after setting up
 * the stack and clearing BSS.
 *
 * This function initializes all subsystems and starts the shell.
 */

#include "uart.h"
#include "memory.h"
#include "string.h"
#include "shell.h"
#include "../filesystem/memfs.h"

/*
 * kernel_main - Main kernel entry point
 *
 * Called from boot.S after basic hardware initialization.
 * This function never returns.
 */
void kernel_main(void) {
    /*
     * Step 1: Initialize UART for console I/O
     * We need this first so we can print status messages
     */
    uart_init();

    /*
     * Print welcome banner
     */
    uart_puts("\n");
    uart_puts("========================================\n");
    uart_puts("          MyOS - ARM64 Edition         \n");
    uart_puts("========================================\n");
    uart_puts("\n");

    /*
     * Step 2: Initialize memory allocator
     */
    uart_puts("[INIT] Initializing memory allocator...\n");
    memory_init();

    /*
     * Step 3: Initialize file system
     */
    uart_puts("[INIT] Initializing file system...\n");
    fs_init();

    /*
     * Step 4: Create some sample files for demonstration
     */
    uart_puts("[INIT] Creating sample files...\n");
    uart_puts("[DEBUG] About to create files...\n");

    int result1 = fs_write_file("welcome.txt", "Welcome to MyOS! This is a sample file.");
    uart_puts("[DEBUG] welcome.txt result: ");
    uart_putc(result1 == 0 ? 'O' : 'F');
    uart_putc('K');
    uart_putc('\n');

    int result2 = fs_write_file("readme.txt", "MyOS is an educational operating system written in ARM64 assembly and C.");
    uart_puts("[DEBUG] readme.txt result: ");
    uart_putc(result2 == 0 ? 'O' : 'F');
    uart_putc('K');
    uart_putc('\n');

    int result3 = fs_write_file("about.txt", "Built for learning OS development concepts.");
    uart_puts("[DEBUG] about.txt result: ");
    uart_putc(result3 == 0 ? 'O' : 'F');
    uart_putc('K');
    uart_putc('\n');

    uart_puts("[DEBUG] All files created.\n");

    /*
     * Step 5: Print system information
     */
    uart_puts("\n");
    uart_puts("[INFO] System ready!\n");
    uart_puts("[INFO] Type 'help' for available commands.\n");
    uart_puts("[INFO] Type 'ls' to see sample files.\n");

    /*
     * Step 6: Start the interactive shell
     * This function never returns
     */
    shell_run();

    /*
     * We should never reach here, but if we do, halt
     */
    while (1) {
        // Infinite loop
    }
}
