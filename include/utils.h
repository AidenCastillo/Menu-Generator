#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define UNUSED __attribute__((unused)) // Ignore unused parameter warning

// Remove leading whitespace from a string
static inline char* ltrim(char* s) {
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

#endif // UTILS_H
