/*
 * UART Driver Implementation
 *
 * This driver communicates with the PL011 UART hardware in QEMU.
 * The UART is memory-mapped at address 0x09000000.
 */

#include "uart.h"

/*
 * UART base address for QEMU's virt machine
 * This is where the UART hardware registers are mapped in memory
 */
#define UART_BASE 0x09000000

/*
 * UART register offsets
 * These are the different control registers for the UART
 */
#define UART_DR     (*(volatile uint32_t*)(UART_BASE + 0x00))  // Data Register
#define UART_FR     (*(volatile uint32_t*)(UART_BASE + 0x18))  // Flag Register
#define UART_IBRD   (*(volatile uint32_t*)(UART_BASE + 0x24))  // Integer Baud Rate
#define UART_FBRD   (*(volatile uint32_t*)(UART_BASE + 0x28))  // Fractional Baud Rate
#define UART_LCRH   (*(volatile uint32_t*)(UART_BASE + 0x2C))  // Line Control
#define UART_CR     (*(volatile uint32_t*)(UART_BASE + 0x30))  // Control Register

/*
 * UART Flag Register bits
 */
#define UART_FR_TXFF (1 << 5)  // Transmit FIFO Full
#define UART_FR_RXFE (1 << 4)  // Receive FIFO Empty

/*
 * Initialize the UART
 *
 * For QEMU, minimal initialization is needed as QEMU sets up
 * most of the UART for us. We just enable transmission and reception.
 */
void uart_init(void) {
    /*
     * Disable UART while we configure it
     */
    UART_CR = 0;

    /*
     * Set baud rate to 115200
     * These values are calculated based on the UART clock frequency
     * For QEMU, these are reasonable defaults
     */
    UART_IBRD = 26;   // Integer part
    UART_FBRD = 3;    // Fractional part

    /*
     * Configure line control register
     * 8 bits, no parity, 1 stop bit, FIFOs enabled
     */
    UART_LCRH = (3 << 5) | (1 << 4);  // 8 bits, FIFO enable

    /*
     * Enable UART, transmission, and reception
     */
    UART_CR = (1 << 0) |  // UART enable
              (1 << 8) |  // TX enable
              (1 << 9);   // RX enable
}

/*
 * Write a single character to UART
 *
 * We wait until the transmit FIFO is not full, then write the character
 */
void uart_putc(char c) {
    /*
     * Wait until TX FIFO is not full
     * UART_FR_TXFF = 1 when FIFO is full
     */
    while (UART_FR & UART_FR_TXFF) {
        // Busy wait
    }

    /*
     * Write the character to the data register
     */
    UART_DR = (uint32_t)c;

    /*
     * Handle newline: also send carriage return for proper formatting
     * This makes output look correct in terminal emulators
     */
    if (c == '\n') {
        while (UART_FR & UART_FR_TXFF) {
            // Busy wait
        }
        UART_DR = '\r';
    }
}

/*
 * Write a null-terminated string to UART
 */
void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

/*
 * Read a single character from UART
 *
 * This blocks until a character is available
 */
char uart_getc(void) {
    /*
     * Wait until RX FIFO is not empty
     * UART_FR_RXFE = 1 when FIFO is empty
     */
    while (UART_FR & UART_FR_RXFE) {
        // Busy wait
    }

    /*
     * Read and return the character
     * Only the lowest 8 bits contain the actual character
     */
    return (char)(UART_DR & 0xFF);
}

/*
 * Check if a character is available to read
 */
int uart_can_read(void) {
    /*
     * Return 1 if FIFO is NOT empty (data available)
     * Return 0 if FIFO is empty
     */
    return !(UART_FR & UART_FR_RXFE);
}

/*
 * Read a line of text from UART
 *
 * Reads until newline or buffer is full
 * Handles backspace for editing
 */
int uart_gets(char *buffer, int max_len) {
    int pos = 0;
    char c;

    while (pos < max_len - 1) {
        c = uart_getc();

        /*
         * Handle backspace (ASCII 8 or 127)
         */
        if (c == 8 || c == 127) {
            if (pos > 0) {
                pos--;
                // Send backspace, space, backspace to erase on screen
                uart_putc(8);
                uart_putc(' ');
                uart_putc(8);
            }
            continue;
        }

        /*
         * Handle newline/carriage return
         */
        if (c == '\n' || c == '\r') {
            uart_putc('\n');  // Echo newline
            buffer[pos] = '\0';
            return pos;
        }

        /*
         * Regular character - add to buffer and echo
         */
        buffer[pos++] = c;
        uart_putc(c);  // Echo the character
    }

    /*
     * Buffer full
     */
    buffer[pos] = '\0';
    return pos;
}
