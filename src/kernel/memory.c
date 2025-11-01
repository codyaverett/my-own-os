/*
 * Memory Allocator Implementation
 *
 * This is a simple "bump allocator" - it just increments a pointer
 * for each allocation. It doesn't support freeing individual blocks.
 *
 * For an educational OS, this is sufficient. A real OS would use
 * a more sophisticated allocator like a free-list or slab allocator.
 */

#include "memory.h"
#include "string.h"

/*
 * Heap boundaries defined in linker.ld
 */
extern char __heap_start;
extern char __heap_end;

/*
 * Current position in the heap
 */
static char *heap_current = NULL;
static size_t total_allocated = 0;

/*
 * Initialize the memory allocator
 */
void memory_init(void) {
    heap_current = &__heap_start;
    total_allocated = 0;
}

/*
 * malloc - Allocate memory
 *
 * This is a bump allocator: we just move the heap pointer forward.
 * We don't support freeing individual allocations.
 */
void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    /*
     * Align to 16 bytes for ARM64
     * ARM64 requires 16-byte alignment for certain operations
     */
    size = (size + 15) & ~15;

    /*
     * Check if we have enough space
     */
    if (heap_current + size > &__heap_end) {
        return NULL;  // Out of memory
    }

    /*
     * Allocate the memory
     */
    void *ptr = heap_current;
    heap_current += size;
    total_allocated += size;

    return ptr;
}

/*
 * free - Free memory
 *
 * Our simple bump allocator doesn't support freeing.
 * In a real OS, this would return memory to a free list.
 * We keep the function for API compatibility.
 */
void free(void *ptr) {
    /*
     * Do nothing in bump allocator
     * This is a limitation of our simple design
     */
    (void)ptr;  // Suppress unused parameter warning
}

/*
 * calloc - Allocate and zero memory
 */
void *calloc(size_t num, size_t size) {
    size_t total = num * size;
    void *ptr = malloc(total);

    if (ptr != NULL) {
        memset(ptr, 0, total);
    }

    return ptr;
}

/*
 * Get total allocated memory
 */
size_t get_allocated_memory(void) {
    return total_allocated;
}
