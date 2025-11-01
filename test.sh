#!/bin/bash

# Run QEMU for 3 seconds and capture output
timeout 3 qemu-system-aarch64 -M virt -cpu cortex-a57 -kernel kernel.elf -nographic 2>&1 || true
