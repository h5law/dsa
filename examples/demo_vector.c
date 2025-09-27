/* $dsa: demo application of the vector */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <dsa/vector.h>

int main(void)
{
    struct vector *vec = vector_init();
    uintptr_t ptr = 0;
    uint64_t res = 0;

    uint64_t val = 0xDEADBEEF;
    for (size_t i = 0; i < 47; ++i) {
       vector_set(vec, ( uintptr_t )&val, i);
    }
    assert(vec->capacity == 64);
    vector_set(vec, ( uintptr_t )&val, 47);
    assert(vec->capacity == 128);
    assert(vec->count == 48);

    ptr = vector_get(vec, 24);
    res = *((uint64_t *)(uintptr_t)ptr);
    assert(res == 0xDEADBEEF);

    vector_deinit(vec, 0);

    return 0;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
