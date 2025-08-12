#ifndef UTILS_H
#define UTILS_H

#include <string.h> // for strncpy (Arduino-compatible)

/**
 * @brief Safely copies a C-string into a fixed-size character buffer.
 * 
 * Ensures the destination string is always null-terminated and prevents buffer overflows
 * by copying at most (destSize - 1) characters from the source string.
 * 
 * @param dest Pointer to the destination character array.
 * @param src  Null-terminated source string to copy.
 * @param destSize Size of the destination buffer in bytes.
 */
inline void safe_strcpy(char* dest, const char* src, size_t destSize) {
    if (destSize == 0) return;                     // Prevent writing to a zero-size buffer
    strncpy(dest, src, destSize - 1);              // Copy at most destSize - 1 characters
    dest[destSize - 1] = '\0';                     // Always null-terminate
}

#endif // UTILS_H