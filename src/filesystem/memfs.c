/*
 * In-Memory File System Implementation
 *
 * This file system keeps all files in RAM using a simple array.
 * It's not persistent - files are lost when the OS restarts.
 */

#include "memfs.h"
#include "../kernel/memory.h"
#include "../kernel/string.h"
#include "../kernel/uart.h"  // For debug output

/*
 * Array of files
 * This is our entire file system - just an array in memory
 */
static file_t files[MAX_FILES];

/*
 * Initialize the file system
 */
void fs_init(void) {
    /*
     * Mark all file slots as unused
     */
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].in_use = 0;
        files[i].content = NULL;
        files[i].size = 0;
        files[i].name[0] = '\0';
    }
}

/*
 * Find a file by name
 * Returns pointer to file, or NULL if not found
 */
static file_t *find_file(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].in_use && strcmp(files[i].name, filename) == 0) {
            return &files[i];
        }
    }
    return NULL;
}

/*
 * Find a free file slot
 * Returns pointer to file slot, or NULL if file system is full
 */
static file_t *find_free_slot(void) {
    uart_puts("[FS_DEBUG] find_free_slot: entering loop\n");
    for (int i = 0; i < MAX_FILES; i++) {
        uart_puts("[FS_DEBUG] find_free_slot: checking slot ");
        uart_putc('0' + (i / 10));
        uart_putc('0' + (i % 10));
        uart_putc('\n');

        if (!files[i].in_use) {
            uart_puts("[FS_DEBUG] find_free_slot: found free slot\n");
            return &files[i];
        }
    }
    uart_puts("[FS_DEBUG] find_free_slot: no free slots\n");
    return NULL;
}

/*
 * Create or update a file
 */
int fs_write_file(const char *filename, const char *content) {
    uart_puts("[FS_DEBUG] fs_write_file called\n");

    /*
     * Validate filename
     */
    if (filename == NULL || filename[0] == '\0') {
        uart_puts("[FS_DEBUG] Invalid filename\n");
        return -1;  // Invalid filename
    }

    uart_puts("[FS_DEBUG] About to call strlen on filename\n");
    if (strlen(filename) >= MAX_FILENAME_LEN) {
        uart_puts("[FS_DEBUG] Filename too long\n");
        return -1;  // Filename too long
    }

    uart_puts("[FS_DEBUG] About to call strlen on content\n");
    /*
     * Validate content size
     */
    size_t content_len = (content != NULL) ? strlen(content) : 0;
    uart_puts("[FS_DEBUG] strlen completed\n");
    if (content_len > MAX_FILE_SIZE) {
        uart_puts("[FS_DEBUG] Content too large\n");
        return -1;  // Content too large
    }

    /*
     * Try to find existing file
     */
    uart_puts("[FS_DEBUG] About to call find_file\n");
    file_t *file = find_file(filename);
    uart_puts("[FS_DEBUG] find_file returned\n");

    /*
     * If file doesn't exist, create it
     */
    if (file == NULL) {
        uart_puts("[FS_DEBUG] File not found, creating new\n");
        file = find_free_slot();
        if (file == NULL) {
            uart_puts("[FS_DEBUG] No free slots!\n");
            return -1;  // File system full
        }

        uart_puts("[FS_DEBUG] Got free slot, calling strncpy\n");
        // Initialize new file
        strncpy(file->name, filename, MAX_FILENAME_LEN - 1);
        uart_puts("[FS_DEBUG] strncpy done\n");
        file->name[MAX_FILENAME_LEN - 1] = '\0';
        file->in_use = 1;
    } else {
        // File exists - free old content
        if (file->content != NULL) {
            free(file->content);
        }
    }

    /*
     * Allocate and copy content
     */
    uart_puts("[FS_DEBUG] About to allocate content\n");
    if (content_len > 0) {
        file->content = (char *)malloc(content_len + 1);
        uart_puts("[FS_DEBUG] malloc returned\n");
        if (file->content == NULL) {
            // Out of memory - mark slot as unused
            uart_puts("[FS_DEBUG] malloc failed!\n");
            file->in_use = 0;
            return -1;
        }
        uart_puts("[FS_DEBUG] About to strcpy content\n");
        strcpy(file->content, content);
        uart_puts("[FS_DEBUG] strcpy done\n");
    } else {
        file->content = NULL;
    }

    file->size = content_len;

    uart_puts("[FS_DEBUG] fs_write_file complete\n");
    return 0;  // Success
}

/*
 * Read a file's content
 */
const char *fs_read_file(const char *filename) {
    file_t *file = find_file(filename);

    if (file == NULL) {
        return NULL;  // File not found
    }

    return file->content;
}

/*
 * Delete a file
 */
int fs_delete_file(const char *filename) {
    file_t *file = find_file(filename);

    if (file == NULL) {
        return -1;  // File not found
    }

    /*
     * Free the content and mark slot as unused
     */
    if (file->content != NULL) {
        free(file->content);
        file->content = NULL;
    }

    file->in_use = 0;
    file->size = 0;
    file->name[0] = '\0';

    return 0;  // Success
}

/*
 * List all files
 */
void fs_list_files(void (*callback)(const char *name, size_t size)) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].in_use) {
            callback(files[i].name, files[i].size);
        }
    }
}

/*
 * Check if a file exists
 */
int fs_file_exists(const char *filename) {
    return (find_file(filename) != NULL) ? 1 : 0;
}

/*
 * Get number of files
 */
int fs_get_file_count(void) {
    int count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].in_use) {
            count++;
        }
    }
    return count;
}
