# Quick Start Guide

## Your OS is Ready!

The build was successful! Here's how to run and test your operating system.

## Running MyOS

```bash
./run.sh
```

You should see:

```
========================================
          MyOS - ARM64 Edition
========================================

[INIT] Initializing memory allocator...
[INIT] Initializing file system...
[INIT] Creating sample files...

[INFO] System ready!
[INFO] Type 'help' for available commands.
[INFO] Type 'ls' to see sample files.

========================================
       Welcome to MyOS Shell!
========================================

Type 'help' for available commands.

myos>
```

## Testing Commands

Try these commands:

### 1. See available commands
```
myos> help
```

### 2. List files
```
myos> ls
```

### 3. Read a file
```
myos> cat welcome.txt
```

### 4. Create a new file
```
myos> edit hello.txt Hello World from MyOS
```

### 5. View your new file
```
myos> cat hello.txt
```

### 6. Echo text
```
myos> echo This is my custom OS
```

### 7. Delete a file
```
myos> rm hello.txt
```

### 8. Clear the screen
```
myos> clear
```

## Exiting QEMU

To exit QEMU:
1. Press `Ctrl-A` (release both keys)
2. Press `X`

## What's Included

Your OS has 3 sample files created at boot:
- `welcome.txt` - Welcome message
- `readme.txt` - About the OS
- `about.txt` - Information about the project

## Learn More

- `docs/ARCHITECTURE.md` - How the OS works internally
- `docs/BUILD.md` - Build system details
- `docs/COMMANDS.md` - Complete command reference

## Rebuilding

After making changes to the source code:

```bash
make clean   # Remove old build artifacts
make         # Rebuild
./run.sh     # Run
```

Enjoy exploring your custom ARM64 operating system!
