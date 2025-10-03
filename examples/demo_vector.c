/* $dsa: demo application of the vector */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <dsa/vector.h>

int main(void)
{
    uintptr_t ptr = 0;
    uint64_t res = 0;
    uint64_t val = 0xDEADBEEF;

    printf("Testing vector_init\n");
    struct vector *vec = vector_init();
    assert(vec != NULL);

    printf("Testing vector_set up to load factor\n");
    assert(vec->count == 0 && vec->capacity == 64);
    for (size_t i = 0; i < 47; ++i) {
        int rc = vector_set(vec, (uintptr_t)(void *)&val, i);
        assert(vec->count == i+1 && vec->capacity == 64);
    }
    assert(vec->count == 47);
    assert(vec->capacity == 64);

    printf("Testing vector_set grows in capacity at load factor\n");
    assert(vector_set(vec, ( uintptr_t )&val, 47) == 0);
    assert(vec->capacity == 128);
    assert(vec->count == 48);
    printf("Old cap: %zu\tNew cap: %zu\tCount: %zu\n", VECTOR_DEFAULT_CAPACITY, vec->capacity, vec->count);

    printf("Testing get produces correct value\n");
    ptr = vector_get(vec, 24);
    assert(ptr != 0);
    res = *((uint64_t *)(uintptr_t)ptr);
    assert(res == 0xDEADBEEF);

    vector_deinit(vec, 0);

    return 0;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
