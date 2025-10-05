/* $dsa: map.h */

#ifndef HASHMAP_H
#define HASHMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

struct bucket {
    struct bucket *next;
    const void    *key;
    size_t         key_len;
    uint64_t       hash;
    uintptr_t      value;
};

struct hashmap {
    struct vector *vec;
    size_t         size;
};

/* Hasher functions */
uint64_t fnv1a_block_hash(const unsigned char *data, size_t len);
#define HASH_REDUCE(hash, vec)                                                 \
    ((( uint64_t )hash) & ( uint64_t )((( struct vector * )vec->capacity - 1)))

struct bucket new_item(const void *key, size_t key_len, uintptr_t value);

struct hashmap *hashmap_init(void);
int hashmap_set(struct hashmap *map, const unsigned char *key, size_t key_len,
                uintptr_t value);
uintptr_t hashmap_get(struct hashmap *map, const unsigned char *key,
                      size_t key_len);
void      hashmap_deinit(struct hashmap *map, int dealloc);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
