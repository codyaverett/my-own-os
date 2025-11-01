/*
 * String Utilities Implementation
 *
 * Basic implementations of standard C string functions.
 * We need these because we don't have access to libc.
 */

#include "string.h"

/*
 * strlen - Calculate the length of a string
 */
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

/*
 * strcmp - Compare two strings
 * Returns: 0 if equal, negative if s1 < s2, positive if s1 > s2
 */
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

/*
 * strncmp - Compare up to n characters of two strings
 */
int strncmp(const char *s1, const char *s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

/*
 * strcpy - Copy string src to dst
 */
char *strcpy(char *dst, const char *src) {
    char *original_dst = dst;
    while ((*dst++ = *src++) != '\0') {
        // Copy until null terminator
    }
    return original_dst;
}

/*
 * strncpy - Copy up to n characters
 */
char *strncpy(char *dst, const char *src, size_t n) {
    char *original_dst = dst;
    while (n > 0 && *src != '\0') {
        *dst++ = *src++;
        n--;
    }
    while (n > 0) {
        *dst++ = '\0';
        n--;
    }
    return original_dst;
}

/*
 * strcat - Concatenate src to dst
 */
char *strcat(char *dst, const char *src) {
    char *original_dst = dst;

    // Find end of dst
    while (*dst != '\0') {
        dst++;
    }

    // Copy src to end of dst
    while ((*dst++ = *src++) != '\0') {
        // Copy until null terminator
    }

    return original_dst;
}

/*
 * strchr - Find first occurrence of character c
 */
char *strchr(const char *str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;
    }
    return NULL;
}

/*
 * memset - Fill memory with a constant byte
 */
void *memset(void *ptr, int value, size_t num) {
    unsigned char *p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

/*
 * memcpy - Copy memory from src to dst
 */
void *memcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    while (n--) {
        *d++ = *s++;
    }

    return dst;
}

/*
 * memcmp - Compare memory regions
 */
int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (n--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }

    return 0;
}
