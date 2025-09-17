/* $dsa: macros.h */

#ifndef MACROS_H
#define MACROS_H

#ifndef NO_DEBUG
#include <stdio.h>
#define DEBUG(...)                                                             \
    do {                                                                       \
        fprintf(stderr, "%d: %s    ", __LINE__, __FILE__);                     \
        fprintf(stderr, __VA_ARGS__);                                          \
    } while (0);
#else
#define NO_DEBUG
#define DEBUG(...)
#endif

#endif /* MACROS_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
