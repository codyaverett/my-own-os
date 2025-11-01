/*
 * UART Driver Header
 *
 * UART (Universal Asynchronous Receiver/Transmitter) is a serial
 * communication protocol used for console input/output.
 *
 * In QEMU's virt machine, the UART is mapped at address 0x09000000
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

/*
 * Initialize the UART hardware
 * Must be called before any other UART functions
 */
void uart_init(void);

/*
 * Write a single character to the UART
 */
void uart_putc(char c);

/*
 * Write a null-terminated string to the UART
 */
void uart_puts(const char *str);

/*
 * Read a single character from the UART
 * Blocks until a character is available
 */
char uart_getc(void);

/*
 * Read a line of text (until newline or max length)
 * Buffer should be at least max_len bytes
 * Returns the number of characters read
 */
int uart_gets(char *buffer, int max_len);

/*
 * Check if a character is available to read
 * Returns 1 if data is available, 0 otherwise
 */
int uart_can_read(void);

#endif // UART_H
