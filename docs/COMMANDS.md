# Command Reference

This document describes all available commands in the MyOS shell.

## Command Overview

| Command | Description | Example |
|---------|-------------|---------|
| `help` | Show available commands | `help` |
| `clear` | Clear the screen | `clear` |
| `echo` | Print text to console | `echo Hello World` |
| `ls` | List all files | `ls` |
| `cat` | Display file contents | `cat readme.txt` |
| `edit` | Create or edit a file | `edit test.txt Hello` |
| `rm` | Delete a file | `rm test.txt` |

---

## Detailed Command Descriptions

### `help`

Display a list of available commands and their usage.

**Syntax:**
```
help
```

**Example:**
```
myos> help

Available commands:
  help              - Show this help message
  clear             - Clear the screen
  echo <text>       - Print text to console
  ls                - List all files
  cat <filename>    - Display file contents
  edit <file> <txt> - Create/edit a file
  rm <filename>     - Delete a file
```

**Notes:**
- Takes no arguments
- Always available

---

### `clear`

Clear the terminal screen and move cursor to top-left.

**Syntax:**
```
clear
```

**Example:**
```
myos> clear
[Screen is cleared]
```

**Implementation:**
- Uses ANSI escape sequence: `\033[2J\033[H`
- `\033[2J` - Clear entire screen
- `\033[H` - Move cursor to home position (1,1)

**Notes:**
- Works in most terminal emulators
- Does not affect file system or system state

---

### `echo`

Print text to the console.

**Syntax:**
```
echo <text>
```

**Arguments:**
- `<text>` - Any text to display (can include spaces)

**Examples:**
```
myos> echo Hello
Hello

myos> echo Hello World
Hello World

myos> echo Testing 1 2 3
Testing 1 2 3
```

**Notes:**
- Multiple words are separated by spaces
- No special character interpretation (no escape sequences)
- Always adds a newline at the end

---

### `ls`

List all files in the file system.

**Syntax:**
```
ls
```

**Example Output:**
```
myos> ls
Files:
  welcome.txt (40 bytes)
  readme.txt (75 bytes)
  about.txt (48 bytes)
```

**No files:**
```
myos> ls
No files.
```

**Notes:**
- Shows filename and size in bytes
- Files are listed in the order they were created
- Maximum 32 files supported

---

### `cat`

Display the contents of a file.

**Syntax:**
```
cat <filename>
```

**Arguments:**
- `<filename>` - Name of the file to display

**Example:**
```
myos> cat welcome.txt
Welcome to MyOS! This is a sample file.

myos> cat nonexistent.txt
Error: File 'nonexistent.txt' not found.
```

**Notes:**
- Filename is case-sensitive
- Entire file contents are displayed
- No pagination or scrolling

---

### `edit`

Create a new file or overwrite an existing file.

**Syntax:**
```
edit <filename> <content>
```

**Arguments:**
- `<filename>` - Name of the file to create/edit
- `<content>` - Text content to write (can include spaces)

**Examples:**

**Create a new file:**
```
myos> edit hello.txt Hello World
File 'hello.txt' saved.

myos> cat hello.txt
Hello World
```

**Overwrite existing file:**
```
myos> edit hello.txt New content here
File 'hello.txt' saved.

myos> cat hello.txt
New content here
```

**Limitations:**
- Maximum filename length: 63 characters
- Maximum content size: 4095 bytes
- Cannot edit partial content - always replaces entire file
- No line editing or multi-line support

**Error Conditions:**
```
myos> edit
Usage: edit <filename> <content>

myos> edit verylongfilenamethatexceedsthelimit content
Error: Could not save file.
```

---

### `rm`

Delete a file from the file system.

**Syntax:**
```
rm <filename>
```

**Arguments:**
- `<filename>` - Name of the file to delete

**Examples:**

**Delete existing file:**
```
myos> ls
Files:
  test.txt (12 bytes)

myos> rm test.txt
File 'test.txt' deleted.

myos> ls
No files.
```

**File not found:**
```
myos> rm nonexistent.txt
Error: File 'nonexistent.txt' not found.
```

**Notes:**
- Deletion is immediate and permanent (no trash/undo)
- Memory is freed for reuse
- Cannot delete directories (no directory support)

---

## Usage Tips

### 1. File Naming

**Best practices:**
- Use descriptive names: `notes.txt`, `config.dat`
- Avoid special characters
- Use extensions for clarity (.txt, .md, .dat)

**Valid filenames:**
```
hello.txt
my_notes
test123
data.csv
```

### 2. Working with Multiple Files

```
# Create several files
myos> edit file1.txt First file
myos> edit file2.txt Second file
myos> edit file3.txt Third file

# List all files
myos> ls
Files:
  file1.txt (10 bytes)
  file2.txt (11 bytes)
  file3.txt (10 bytes)

# View specific file
myos> cat file2.txt
Second file
```

### 3. File Content Best Practices

**Short content works best:**
```
myos> edit note.txt Remember to test the shell
```

**Multiple words are allowed:**
```
myos> edit story.txt Once upon a time in a land far away
```

**Special characters:**
Most special characters work fine, but avoid:
- Null bytes (`\0`)
- Control characters

### 4. Common Workflows

**Check existing files before creating:**
```
myos> ls
myos> edit myfile.txt content here
myos> ls
```

**Verify file content after editing:**
```
myos> edit config.txt setting=value
myos> cat config.txt
setting=value
```

**Clean up temporary files:**
```
myos> rm temp.txt
myos> rm old_data.txt
```

## Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| `Unknown command: xyz` | Command doesn't exist | Type `help` to see available commands |
| `Usage: cat <filename>` | Missing arguments | Provide required arguments |
| `Error: File 'x' not found` | File doesn't exist | Check filename with `ls` |
| `Error: Could not save file` | File system full or filename too long | Delete files or use shorter name |

## Limitations

1. **No Pipes or Redirection**
   - Cannot do: `cat file.txt | echo`
   - Each command runs independently

2. **No Command History**
   - Cannot use up/down arrows to recall commands
   - Must retype commands

3. **No Tab Completion**
   - Cannot press Tab to auto-complete filenames

4. **No Background Jobs**
   - All commands run in foreground
   - No job control

5. **No Directories**
   - Flat file system structure
   - All files in root directory

6. **No Permissions**
   - No file ownership or permissions
   - All files readable/writable by everyone

## Future Enhancements

Potential improvements for learning:

- Add command history (up/down arrow keys)
- Implement tab completion
- Add a `cp` (copy) command
- Add a `mv` (move/rename) command
- Support directories with `mkdir`, `cd`, `pwd`
- Add text editor mode for multi-line editing
- Implement basic permissions system

## Getting Help

If a command isn't working:

1. Type `help` to see correct syntax
2. Check `ARCHITECTURE.md` for system design
3. Review `BUILD.md` for build issues
4. Examine source code in `src/kernel/shell.c`
