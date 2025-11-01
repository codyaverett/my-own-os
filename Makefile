# Makefile for ARM64 Operating System

# Cross-compiler toolchain
# On macOS: brew install aarch64-elf-gcc
# On Linux: apt-get install gcc-aarch64-linux-gnu

# Try to detect the toolchain prefix
ifeq ($(shell which aarch64-elf-gcc 2>/dev/null),)
    # If aarch64-elf-gcc not found, try aarch64-linux-gnu-gcc
    PREFIX = aarch64-linux-gnu-
else
    PREFIX = aarch64-elf-
endif

CC = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

# Compiler flags
CFLAGS = -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -O2 -std=c11
ASFLAGS =
LDFLAGS = -T src/linker.ld -nostdlib

# Source files
ASM_SOURCES = src/boot/boot.S
C_SOURCES = src/kernel/main.c \
            src/kernel/uart.c \
            src/kernel/memory.c \
            src/kernel/string.c \
            src/kernel/shell.c \
            src/filesystem/memfs.c

# Object files
ASM_OBJECTS = $(ASM_SOURCES:.S=.o)
C_OBJECTS = $(C_SOURCES:.c=.o)
ALL_OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Output
KERNEL = kernel.elf

# Default target
all: $(KERNEL)
	@echo "Build complete! Run with: ./run.sh"

# Link kernel
$(KERNEL): $(ALL_OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(ALL_OBJECTS) -o $(KERNEL)
	@echo "Creating disassembly..."
	$(OBJDUMP) -D $(KERNEL) > kernel.dump

# Compile C sources
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble assembly sources
%.o: %.S
	@echo "Assembling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Run in QEMU
run: $(KERNEL)
	@./run.sh

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -f $(ALL_OBJECTS) $(KERNEL) kernel.dump kernel.bin

# Show help
help:
	@echo "MyOS Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the kernel (default)"
	@echo "  make run      - Build and run in QEMU"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Tools required:"
	@echo "  - ARM64 cross-compiler ($(PREFIX)gcc)"
	@echo "  - QEMU (qemu-system-aarch64)"

.PHONY: all run clean help
