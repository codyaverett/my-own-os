# MyOS - ARM64 Assembly Operating System

A simple educational operating system written in ARM64 assembly and C, designed to run on QEMU. This OS features a basic in-memory file system and an interactive command shell.

## Features

- **ARM64 Architecture**: Native AArch64 bare-metal implementation
- **Interactive Shell**: Command-line interface with built-in commands
- **In-Memory File System**: Simple file storage without disk persistence
- **UART Console**: Serial communication for input/output
- **Educational Focus**: Extensively commented code for learning

## Supported Commands

- `ls` - List all files
- `cat <filename>` - Display file contents
- `edit <filename> <content>` - Create or edit a file
- `rm <filename>` - Delete a file
- `echo <text>` - Print text to console
- `help` - Show available commands
- `clear` - Clear the screen

## Prerequisites

### macOS (recommended for this project)

Install the ARM64 cross-compilation toolchain:

```bash
brew install aarch64-elf-gcc
brew install qemu
```

### Linux

```bash
sudo apt-get install gcc-aarch64-linux-gnu
sudo apt-get install qemu-system-arm
```

## Building

```bash
make
```

This will produce `kernel.elf` - the OS kernel image.

## Running

Launch the OS in QEMU:

```bash
./run.sh
```

Or manually:

```bash
qemu-system-aarch64 -M virt -cpu cortex-a57 -kernel kernel.elf -nographic
```

To exit QEMU: Press `Ctrl-A` then `X`

## Project Structure

```
my-own-os/
├── README.md              # This file
├── Makefile               # Build system
├── run.sh                 # QEMU launcher script
├── docs/                  # Documentation
│   ├── ARCHITECTURE.md    # System design
│   ├── BUILD.md           # Build guide
│   └── COMMANDS.md        # Command reference
├── src/
│   ├── boot/
│   │   └── boot.S         # ARM64 bootloader
│   ├── kernel/
│   │   ├── main.c         # Kernel entry point
│   │   ├── uart.c/h       # Serial console driver
│   │   ├── memory.c/h     # Memory allocator
│   │   ├── string.c/h     # String utilities
│   │   └── shell.c/h      # Command shell
│   ├── filesystem/
│   │   └── memfs.c/h      # In-memory file system
│   └── linker.ld          # Linker script
```

## Development

This OS is designed for educational purposes. Each source file contains detailed comments explaining:
- How ARM64 assembly works
- How the kernel initializes
- How system components interact
- How to extend the functionality

See `docs/ARCHITECTURE.md` for system design details.

## Learning Resources

- [ARM Architecture Reference Manual](https://developer.arm.com/documentation/)
- [OSDev Wiki](https://wiki.osdev.org/)
- [ARM64 Assembly Guide](https://developer.arm.com/documentation/102374/latest/)

## License

This project is for educational purposes. Feel free to use and modify as needed.

## Contributing

This is a learning project, but suggestions and improvements are welcome!
