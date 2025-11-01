# Build Guide

This document explains how to build and run MyOS.

## Prerequisites

### macOS

1. **Install ARM64 Cross-Compiler:**
   ```bash
   brew install aarch64-elf-gcc
   ```

   If that package is not available, try:
   ```bash
   brew install aarch64-linux-gnu-gcc
   ```

2. **Install QEMU:**
   ```bash
   brew install qemu
   ```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install gcc-aarch64-linux-gnu
sudo apt-get install qemu-system-arm
```

### Linux (Fedora/RHEL)

```bash
sudo dnf install gcc-aarch64-linux-gnu
sudo dnf install qemu-system-aarch64
```

## Verify Installation

Check that the tools are installed:

```bash
# Check cross-compiler
aarch64-elf-gcc --version
# OR
aarch64-linux-gnu-gcc --version

# Check QEMU
qemu-system-aarch64 --version
```

## Building the OS

### Quick Build

From the project root directory:

```bash
make
```

This will:
1. Assemble `boot.S` into an object file
2. Compile all `.c` files into object files
3. Link everything into `kernel.elf`
4. Generate `kernel.dump` (disassembly for debugging)

### Build Output

After a successful build, you'll see:

```
Assembling src/boot/boot.S...
Compiling src/kernel/main.c...
Compiling src/kernel/uart.c...
Compiling src/kernel/memory.c...
Compiling src/kernel/string.c...
Compiling src/kernel/shell.c...
Compiling src/filesystem/memfs.c...
Linking kernel...
Creating disassembly...
Build complete! Run with: ./run.sh
```

### Build Artifacts

- `kernel.elf` - The OS kernel (ELF executable)
- `kernel.dump` - Disassembly listing (for debugging)
- `*.o` - Object files (intermediate compilation results)

## Running the OS

### Quick Run

```bash
./run.sh
```

### Manual QEMU Launch

```bash
qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a57 \
    -kernel kernel.elf \
    -nographic \
    -serial stdio
```

**QEMU Parameters Explained:**
- `-M virt` - Use the ARM "virt" machine (virtual platform)
- `-cpu cortex-a57` - Emulate ARM Cortex-A57 processor
- `-kernel kernel.elf` - Load our kernel
- `-nographic` - No GUI window, use terminal
- `-serial stdio` - Route serial I/O to terminal

### Exiting QEMU

To exit QEMU, press:
```
Ctrl-A, then X
```

This is a two-step process:
1. Press and release `Ctrl-A`
2. Press `X`

## Cleaning Build Artifacts

Remove all build artifacts:

```bash
make clean
```

This removes:
- `kernel.elf`
- `kernel.dump`
- All `*.o` files

## Troubleshooting

### Problem: "Command not found: aarch64-elf-gcc"

**Solution:** The Makefile automatically detects whether you have `aarch64-elf-gcc` or `aarch64-linux-gnu-gcc`. If neither is found, install the cross-compiler:

```bash
# macOS
brew install aarch64-elf-gcc

# Linux
sudo apt-get install gcc-aarch64-linux-gnu
```

### Problem: "Command not found: qemu-system-aarch64"

**Solution:** Install QEMU:

```bash
# macOS
brew install qemu

# Linux
sudo apt-get install qemu-system-arm
```

### Problem: Build errors about missing headers

**Solution:** Make sure you're building with the cross-compiler, not your system's native compiler. Check the Makefile to ensure it's using the correct toolchain.

### Problem: QEMU shows "Guest has not initialized the display yet"

**Solution:** This is normal if using `-nographic`. Your OS should output to the terminal, not a graphical window.

### Problem: No output from QEMU

**Possible causes:**
1. UART not initialized correctly
2. Wrong memory address for UART
3. Code not reaching `kernel_main`

**Debug steps:**
1. Check `kernel.dump` to verify code layout
2. Use QEMU's `-d` flag for debug output:
   ```bash
   qemu-system-aarch64 -M virt -cpu cortex-a57 -kernel kernel.elf -nographic -d int,cpu_reset
   ```

## Advanced Build Options

### Debug Build

Add debug symbols and disable optimizations:

```bash
make CFLAGS="-Wall -Wextra -ffreestanding -nostdlib -g -O0"
```

### Verbose Build

See full compiler commands:

```bash
make -n
```

### Build for Different ARM CPU

Edit `run.sh` and change the `-cpu` parameter:

```bash
# Cortex-A72
-cpu cortex-a72

# Cortex-A53
-cpu cortex-a53
```

## Next Steps

Once built and running:
1. Try the sample commands (`help`, `ls`, `cat welcome.txt`)
2. Create your own files with `edit`
3. Modify the source code and rebuild
4. Read `ARCHITECTURE.md` to understand the internals
5. Extend the OS with new commands or features

## Build Process Diagram

```
Source Files                 Object Files              Final Binary
============                 ============              ============

boot.S         ─────────►   boot.o          ╲
                                             ├─────►  kernel.elf
main.c         ─────────►   main.o          │
uart.c         ─────────►   uart.o          │
memory.c       ─────────►   memory.o        │
string.c       ─────────►   string.o        ├────► (linker uses
shell.c        ─────────►   shell.o         │        linker.ld)
memfs.c        ─────────►   memfs.o         ╱

                                                       kernel.dump
                                                       (disassembly)
```
