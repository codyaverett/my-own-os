/*
 * In-Memory File System Header
 *
 * A simple file system that stores files in RAM.
 * Files are lost when the OS is restarted.
 */

#ifndef MEMFS_H
#define MEMFS_H

#include <stddef.h>

/*
 * Maximum number of files in the file system
 */
#define MAX_FILES 32

/*
 * Maximum filename length (including null terminator)
 */
#define MAX_FILENAME_LEN 64

/*
 * Maximum file content size
 */
#define MAX_FILE_SIZE 4096

/*
 * File structure
 */
typedef struct {
    char name[MAX_FILENAME_LEN];  // Filename
    char *content;                 // File content (dynamically allocated)
    size_t size;                   // Content size in bytes
    int in_use;                    // 1 if file exists, 0 if slot is free
} file_t;

/*
 * Initialize the file system
 */
void fs_init(void);

/*
 * Create or update a file
 * Returns 0 on success, -1 on error
 */
int fs_write_file(const char *filename, const char *content);

/*
 * Read a file's content
 * Returns pointer to content, or NULL if file not found
 */
const char *fs_read_file(const char *filename);

/*
 * Delete a file
 * Returns 0 on success, -1 if file not found
 */
int fs_delete_file(const char *filename);

/*
 * List all files
 * Calls callback for each file with filename and size
 */
void fs_list_files(void (*callback)(const char *name, size_t size));

/*
 * Check if a file exists
 * Returns 1 if exists, 0 otherwise
 */
int fs_file_exists(const char *filename);

/*
 * Get number of files
 */
int fs_get_file_count(void);

#endif // MEMFS_H
