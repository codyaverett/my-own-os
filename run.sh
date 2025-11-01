#!/bin/bash

#
# QEMU Run Script for MyOS
#
# This script launches the OS in QEMU with appropriate settings
# for the ARM64 virt machine.
#

KERNEL="kernel.elf"

# Check if kernel exists
if [ ! -f "$KERNEL" ]; then
    echo "Error: Kernel not found. Please run 'make' first."
    exit 1
fi

echo "Starting MyOS in QEMU..."
echo "To exit QEMU: Press Ctrl-A then X"
echo ""

# Launch QEMU with ARM64 virt machine
# -M virt: Use the virtual ARM platform
# -cpu cortex-a57: Emulate Cortex-A57 processor
# -kernel: The kernel image to load
# -nographic: No graphical window, serial I/O via terminal

qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a57 \
    -kernel "$KERNEL" \
    -nographic
