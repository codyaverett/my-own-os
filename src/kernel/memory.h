/*
 * Memory Allocator Header
 *
 * Simple memory allocator for our OS.
 * For educational purposes, we implement a basic bump allocator.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/*
 * Initialize the memory allocator
 * Must be called before malloc/free
 */
void memory_init(void);

/*
 * Allocate size bytes of memory
 * Returns pointer to allocated memory, or NULL if failed
 */
void *malloc(size_t size);

/*
 * Free previously allocated memory
 * Note: Our simple allocator doesn't actually free memory,
 * but we keep the interface for future improvements
 */
void free(void *ptr);

/*
 * Allocate memory and zero it
 */
void *calloc(size_t num, size_t size);

/*
 * Get the total amount of allocated memory
 */
size_t get_allocated_memory(void);

#endif // MEMORY_H
