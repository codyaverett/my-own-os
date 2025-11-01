/*
 * ARM64 Bootloader - boot.S
 *
 * This is the entry point of our operating system. When QEMU starts,
 * it begins executing code from the _start label below.
 *
 * ARM64 uses 64-bit registers (x0-x30) and has different execution levels:
 * - EL0: User applications
 * - EL1: Operating System (kernel) - where we'll run
 * - EL2: Hypervisor
 * - EL3: Secure monitor
 */

.section ".text.boot"  // This section goes at the very beginning
.global _start         // Make _start visible to the linker

/*
 * _start - The entry point of our kernel
 *
 * When QEMU loads the kernel, it may start multiple CPU cores.
 * We only want to use one core, so we park the others.
 */
_start:
    /*
     * Get the current CPU core ID
     * The MPIDR_EL1 register contains the core ID
     * We only want core 0 to continue, others should sleep
     */
    mrs     x0, mpidr_el1       // Read core ID register
    and     x0, x0, #0xFF       // Extract only the core ID bits
    cbz     x0, core0_start     // If core 0, jump to core0_start

    /*
     * If we reach here, we're not core 0
     * Park this core in an infinite loop
     */
park_core:
    wfe                         // Wait For Event (low power sleep)
    b       park_core           // Loop forever

/*
 * core0_start - Initialization for the primary CPU core
 *
 * Here we set up the stack and clear the BSS section before
 * jumping to C code.
 */
core0_start:
    /*
     * Set up the stack pointer
     * The stack grows downward in ARM64, so we point to the top
     * __stack_top is defined in linker.ld
     */
    ldr     x0, =__stack_top
    mov     sp, x0              // sp = stack pointer register

    /*
     * Clear the BSS section
     * BSS contains uninitialized global variables that should start at 0
     * __bss_start and __bss_end are defined in linker.ld
     */
    ldr     x0, =__bss_start    // x0 = start address
    ldr     x1, =__bss_end      // x1 = end address

clear_bss:
    cmp     x0, x1              // Compare current position with end
    b.ge    clear_bss_done      // If x0 >= x1, we're done
    str     xzr, [x0], #8       // Store zero at [x0], then x0 += 8
    b       clear_bss           // Loop

clear_bss_done:
    /*
     * Jump to the C kernel
     * We never return from kernel_main, but if we do, park the core
     */
    bl      kernel_main         // Branch with Link to kernel_main

    /*
     * If kernel_main returns (it shouldn't), halt the system
     */
halt:
    wfe                         // Wait for event
    b       halt                // Loop forever
