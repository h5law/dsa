/* $dsa: vector.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "macros.h"

struct vector *vector_init(void)
{
    DEBUG("Initialising vecor, callocating space\n")
    struct vector *vec = calloc(1, sizeof(struct vector));
    vec->buffer        = calloc(1, VECTOR_DEFAULT_CAPACITY * sizeof(uintptr_t));
    vec->capacity      = (size_t)VECTOR_DEFAULT_CAPACITY;
    return vec;
}

int vector_set(struct vector *vec, uintptr_t value, size_t index)
{
    size_t new_count = vec->count + 1;
    if (index > vec->capacity) {
        DEBUG("Vector index out of bounds\n")
        return -1;
    }
    if (new_count >= VECTOR_MAX_CAPACITY) {
        DEBUG("Vector over capacity\n")
        return -2;
    }
    vec->buffer[index] = value;
    if ((4 * new_count >= 3 * vec->capacity) &&
        (vec->capacity << 1) < VECTOR_MAX_CAPACITY) {
        DEBUG("Resizing and replacing vector\n")
        size_t new_cap = vec->capacity << 1;
        vec->buffer    = realloc(vec->buffer, new_cap * sizeof(uintptr_t));
        memset(vec->buffer + (vec->capacity * sizeof(uintptr_t)), 0,
               (new_cap / 2) * sizeof(uintptr_t));
        vec->capacity = new_cap;
    }
    vec->count         = new_count;
    DEBUG("Added value at index %zu\n", index)
    return 0;
}

uintptr_t vector_get(struct vector *vec, size_t index)
{
    if (index >= vec->capacity) {
        DEBUG("Vector index out of bounds\n")
        return 1;
    }
    DEBUG("Returning value at %zu\n", index)
    return vec->buffer[index];
}

int vector_delete(struct vector *vec, size_t index)
{
    if (vec->count < 1) {
        DEBUG("Vector is empty\n")
        return 1;
    }
    if (index >= vec->capacity) {
        DEBUG("Vector index out of bounds\n")
        return 2;
    }
    DEBUG("Freeing and removing item at %zu\n", index)
    free(( void * )vec->buffer[index]);
    vec->buffer[index] = 0;
    --vec->count;
    return 0;
}

void vector_deinit(struct vector *vec, int dealloc)
{
    if (dealloc == 1) {
        DEBUG("Freeing all members of the vector\n")
        for (size_t i = 0; i < vec->capacity; ++i)
            free(( void * )vec->buffer[i]);
    }
    DEBUG("Freeing the vector and its buffer\n")
    free(vec->buffer);
    free(vec);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
