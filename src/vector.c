/* $dsa: vector.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "vector.h"

struct vector *vector_init(void)
{
    struct vector *vec = calloc(1, sizeof(struct vector));
    vec->buffer        = calloc(1, VECTOR_DEFAULT_CAPACITY * sizeof(uintptr_t));
    vec->capacity      = VECTOR_DEFAULT_CAPACITY;
    return vec;
}

int vector_set(struct vector *vec, uintptr_t value, size_t index)
{
    size_t new_count = vec->count + 1;
    if (index > vec->capacity)
        return -1;
    if (new_count > VECTOR_MAX_CAPACITY)
        return -2;
    vec->buffer[index] = value;
    vec->count         = new_count;
    if ((4 * new_count >= 3 * vec->capacity) &&
        (vec->capacity << 1) < VECTOR_MAX_CAPACITY) {
        size_t new_cap = vec->capacity << 1;
        vec->buffer    = realloc(vec->buffer, new_cap * sizeof(uintptr_t));
        memset(vec->buffer + (vec->capacity * sizeof(uintptr_t)), 0,
               (new_cap / 2) * sizeof(uintptr_t));
        vec->capacity = new_cap;
        return 1;
    }
    return 0;
}

uintptr_t vector_get(struct vector *vec, size_t index)
{
    if (index >= vec->capacity)
        return 0;
    return vec->buffer[index];
}

int vector_delete(struct vector *vec, size_t index)
{
    if (vec->count < 1)
        return 0;
    if (index >= vec->capacity)
        return -1;
    free(( void * )vec->buffer[index]);
    vec->buffer[index] = 0;
    --vec->count;
    return 0;
}

void vector_deinit(struct vector *vec, int dealloc)
{
    if (dealloc)
        for (size_t i = 0; i < vec->capacity; ++i)
            free(( void * )vec->buffer[i]);
    free(vec->buffer);
    free(vec);
}

#ifdef DEBUG_TESTS

#include <assert.h>

#define ASSERT(stmt)                                                           \
    do {                                                                       \
        DEBUG("%s\n", #stmt);                                                  \
        assert(stmt);                                                          \
    } while (0)

#define PRINT_VECTOR(vec)                                                      \
    for (size_t i = 0; i < ( size_t )( struct vector * )vec->capacity; ++i)    \
    DEBUG("%lx ", ( uintptr_t )( struct vector * )vec->buffer +                \
                          (i * sizeof(uintptr_t)))

int main(void)
{
    struct vector *vec = vector_init();
    ASSERT(vec != NULL);
    /* PRINT_VECTOR(vec); */

    uint64_t val = 0xDEADBEEF;
    for (size_t i = 0; i < 47; ++i) {
        ASSERT(vector_set(vec, ( uintptr_t )&val, i) >= 0);
    }
    ASSERT(vec->count == 47);
    ASSERT(vec->capacity == 64);
    /* PRINT_VECTOR(vec); */
    ASSERT(vector_set(vec, ( uintptr_t )&val, 47) >= 0);
    ASSERT(vec->capacity == 128);

    vector_deinit(vec, 0);
    ASSERT(vec->buffer == NULL);

    return 0;
}

#endif /* DBEUG_TESTS */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
