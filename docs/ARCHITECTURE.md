# MyOS Architecture

## Overview

MyOS is a simple educational operating system written in ARM64 assembly and C. It demonstrates fundamental OS concepts including bootloading, memory management, device drivers, and file systems.

## System Components

### 1. Bootloader (`src/boot/boot.S`)

The bootloader is the first code that executes when the OS starts.

**Responsibilities:**
- Identify the current CPU core (we only use core 0)
- Park additional cores in low-power mode
- Set up the stack pointer
- Clear the BSS section (uninitialized global variables)
- Jump to the C kernel (`kernel_main`)

**Key ARM64 Concepts:**
- **Registers**: ARM64 has 31 general-purpose 64-bit registers (x0-x30) plus a stack pointer (sp)
- **Execution Levels**: ARM64 has 4 privilege levels (EL0-EL3). Our OS runs at EL1 (kernel mode)
- **MPIDR_EL1**: Register containing the current CPU core ID

### 2. Memory Layout (`src/linker.ld`)

The linker script defines how the kernel is laid out in memory.

```
0x40000000: Kernel start (QEMU default load address)
    .text:   Executable code
    .rodata: Read-only data (strings, constants)
    .data:   Initialized global variables
    .bss:    Uninitialized globals (cleared to zero)
    .stack:  16KB system stack
    .heap:   1MB heap for dynamic allocation
```

### 3. UART Driver (`src/kernel/uart.c`)

Provides console input/output through a serial port.

**Hardware Interface:**
- Uses PL011 UART at address 0x09000000 (QEMU virt machine)
- Memory-mapped I/O: UART registers are accessed via memory addresses
- Supports both reading and writing characters

**Key Functions:**
- `uart_init()`: Configure UART hardware
- `uart_putc()`: Write a single character
- `uart_getc()`: Read a single character (blocking)
- `uart_gets()`: Read a line with backspace support

### 4. Memory Allocator (`src/kernel/memory.c`)

Simple "bump allocator" for dynamic memory.

**How it works:**
- Maintains a pointer to the next free memory location
- Each allocation moves the pointer forward
- Does NOT support freeing individual allocations
- Fast and simple, but limited

**Trade-offs:**
- ✅ Simple to implement
- ✅ Fast allocation (just pointer increment)
- ❌ Cannot free memory
- ❌ Can waste memory

**Future improvements:**
- Implement a free-list allocator
- Add support for `free()` to reuse memory

### 5. In-Memory File System (`src/filesystem/memfs.c`)

Array-based file storage in RAM.

**Data Structure:**
```c
file_t files[MAX_FILES];  // Array of 32 file slots
```

Each file has:
- Name (up to 64 characters)
- Content (dynamically allocated, up to 4KB)
- Size in bytes
- In-use flag

**Limitations:**
- Maximum 32 files
- Maximum 4KB per file
- Not persistent (lost on restart)
- No directories (flat structure)

### 6. Command Shell (`src/kernel/shell.c`)

Interactive command-line interface.

**Command Processing:**
1. Read line from UART
2. Parse into command and arguments
3. Dispatch to appropriate handler
4. Display results

**Supported Commands:**
- `help`: Show available commands
- `clear`: Clear screen (ANSI escape codes)
- `echo <text>`: Print text
- `ls`: List files
- `cat <file>`: Display file contents
- `edit <file> <content>`: Create/edit file
- `rm <file>`: Delete file

## Boot Sequence

1. **QEMU** loads `kernel.elf` at address 0x40000000
2. **Bootloader** (`_start` in boot.S):
   - Check CPU core ID
   - Set up stack
   - Clear BSS
   - Jump to `kernel_main`
3. **Kernel** (`kernel_main` in main.c):
   - Initialize UART
   - Initialize memory allocator
   - Initialize file system
   - Create sample files
   - Start shell
4. **Shell** runs in infinite loop, processing commands

## Memory Management

### Stack

- Size: 16KB
- Grows downward (high to low addresses)
- Used for function calls and local variables
- Must be 16-byte aligned (ARM64 requirement)

### Heap

- Size: 1MB
- Grows upward (low to high addresses)
- Used for dynamic allocations (`malloc`)
- Managed by bump allocator

## I/O Model

**Memory-Mapped I/O:**
- Hardware devices are accessed through memory addresses
- Writing to a memory address sends data to the device
- Reading from a memory address receives data from the device

Example: UART Data Register at 0x09000000
```c
*(volatile uint32_t*)0x09000000 = 'A';  // Send 'A' to UART
char c = *(volatile uint32_t*)0x09000000; // Read from UART
```

## Key Learning Concepts

1. **Bare Metal Programming**: No OS underneath, direct hardware access
2. **Memory Layout**: Understanding how code and data are organized
3. **Device Drivers**: Communicating with hardware through registers
4. **File Systems**: Data structures for organizing files
5. **Command Parsing**: Processing user input into structured commands
6. **ARM64 Assembly**: Low-level CPU programming

## Further Reading

- [ARM Architecture Reference Manual](https://developer.arm.com/documentation/)
- [OSDev Wiki](https://wiki.osdev.org/)
- [QEMU Documentation](https://www.qemu.org/docs/master/)
