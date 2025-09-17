/* $dsa: vector.h */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>

/* Structure declarations */
#define VECTOR_RESIZE_FACTOR    2
#define VECTOR_LOAD_FACTOR      0.75F
#define VECTOR_DEFAULT_CAPACITY 64
#define VECTOR_MAX_CAPACITY     32768

struct vector {
    uintptr_t *buffer;
    size_t     capacity;
    size_t     count;
};

/* Vector functions */
struct vector *vector_init(void);
int            vector_set(struct vector *vec, uintptr_t value, size_t index);
uintptr_t      vector_get(struct vector *vec, size_t index);
int vector_delete(struct vector *vec, size_t index);
void           vector_deinit(struct vector *vec, int dealloc);

#endif /* VECTOR_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
