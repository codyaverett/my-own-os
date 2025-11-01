/*
 * Linker Script for ARM64 Operating System
 *
 * This script defines the memory layout and sections of our kernel.
 * When QEMU loads the kernel, it places it at address 0x40000000 by default.
 */

/* Entry point - the first function to execute */
ENTRY(_start)

SECTIONS
{
    /*
     * Start the kernel at 0x40000000 (1GB mark)
     * This is where QEMU's virt machine loads the kernel by default
     */
    . = 0x40000000;

    /*
     * .text section: Contains executable code
     * This includes both assembly and C code
     */
    .text : {
        *(.text.boot)    /* Boot code goes first */
        *(.text)         /* All other code */
    }

    /*
     * Read-only data section
     * Contains string constants and other immutable data
     */
    .rodata : {
        *(.rodata*)
    }

    /*
     * Data section: Initialized global/static variables
     */
    .data : {
        *(.data)
    }

    /*
     * BSS section: Uninitialized global/static variables
     * These are automatically zeroed by the bootloader
     */
    .bss : {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }

    /*
     * Stack: Reserve 16KB for the system stack
     * Stack grows downward in ARM64
     */
    .stack (NOLOAD) : {
        . = ALIGN(16);        /* Stack must be 16-byte aligned in ARM64 */
        . = . + 0x4000;       /* 16KB stack */
        __stack_top = .;
    }

    /*
     * Heap: Reserve memory for dynamic allocation
     * Starts after the stack
     */
    .heap (NOLOAD) : {
        . = ALIGN(16);
        __heap_start = .;
        . = . + 0x100000;     /* 1MB heap */
        __heap_end = .;
    }
}
