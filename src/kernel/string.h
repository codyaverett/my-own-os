/*
 * String Utilities Header
 *
 * Since we're running bare-metal without a standard library,
 * we need to implement our own string functions.
 */

#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

/*
 * Get the length of a null-terminated string
 */
size_t strlen(const char *str);

/*
 * Compare two strings
 * Returns 0 if equal, non-zero otherwise
 */
int strcmp(const char *s1, const char *s2);

/*
 * Compare up to n characters of two strings
 */
int strncmp(const char *s1, const char *s2, size_t n);

/*
 * Copy string src to dst
 * Returns dst
 */
char *strcpy(char *dst, const char *src);

/*
 * Copy up to n characters from src to dst
 */
char *strncpy(char *dst, const char *src, size_t n);

/*
 * Concatenate src to the end of dst
 */
char *strcat(char *dst, const char *src);

/*
 * Find first occurrence of character c in string
 */
char *strchr(const char *str, int c);

/*
 * Set n bytes of memory to value c
 */
void *memset(void *ptr, int value, size_t num);

/*
 * Copy n bytes from src to dst
 */
void *memcpy(void *dst, const void *src, size_t n);

/*
 * Compare n bytes of two memory regions
 */
int memcmp(const void *s1, const void *s2, size_t n);

#endif // STRING_H
