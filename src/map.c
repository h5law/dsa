/* $dsa: map.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "vector.h"
#include "map.h"

/* FNV-1a standard initial prime offset */
#define HASH_VALUE_OFFSET 0XCBF29CE484222325
/* SplitMix64 highly dispersive odd u64 */
#define HASH_VALUE_PRIME  0XD6E8FEB86659FD93

uint64_t fnv1a_block_hash(const unsigned char *data, size_t len)
{
    size_t   blocks = len / 8;
    uint64_t hash   = HASH_VALUE_OFFSET;
    for (size_t i = 0; i < blocks; ++i) {
        hash ^= *( const uint64_t * )data;
        hash *= HASH_VALUE_PRIME;
        data += 8;
    }

    /* Process any left-overs bytes from the block processor */
    uint64_t last = len & 0xFF;
    switch (len % 8) {
    case 7:
        last |= ( uint64_t )data[6] << 56; /* fallthrough */
    case 6:
        last |= ( uint64_t )data[5] << 48; /* fallthrough */
    case 5:
        last |= ( uint64_t )data[4] << 40; /* fallthrough */
    case 4:
        last |= ( uint64_t )data[3] << 32; /* fallthrough */
    case 3:
        last |= ( uint64_t )data[2] << 24; /* fallthrough */
    case 2:
        last |= ( uint64_t )data[1] << 16; /* fallthrough */
    case 1:
        last |= ( uint64_t )data[0] << 8;
        hash ^= last;
        hash *= HASH_VALUE_PRIME;
    }

    return hash;
}

void add_bucket(struct bucket *b1, struct bucket *b2);
int  find_bucket(struct bucket *b, uint64_t hash);
int  delete_bucket(struct bucket *b, uint64_t hash);

void add_bucket(struct bucket *b1, struct bucket *b2)
{
    struct bucket *b = b1->next;
    if (!b)
        b1->next = b2;
    while (b->next) {
        b = b->next;
    }
    b->next = b2;
}

int find_bucket(struct bucket *b, uint64_t hash)
{
    do {
        if (b->hash == hash)
            return 0;
        b = b->next;
    } while (b->next);
    return -1;
}

int delete_bucket(struct bucket *b, uint64_t hash)
{
    struct bucket *tmp = NULL;
    do {
        if (b->hash == hash)
            goto delete;
        b = b->next;
    } while (b->next);
    return -1;
    delete : tmp = b->next;
    free(( void * )b);
    b = tmp;
    return 0;
}

int hashmap_set(struct hashmap *map, const unsigned char *key, size_t key_len,
                uintptr_t value)
{
    int            rc    = 0;
    uint64_t       hash  = 0;
    size_t         index = 0;
    struct bucket *b     = NULL;

    hash                 = fnv1a_block_hash(key, key_len);
    index                = HASH_REDUCE(hash, map->vec);
    b                    = calloc(1, sizeof(struct bucket));
    if (!b) {
        DEBUG("error creating bucket object for key %s", key);
        return -3;
    }
    b->key        = key;
    b->key_len    = key_len;
    b->hash       = index;
    b->value      = value;

    uintptr_t ptr = vector_get(map->vec, index);
    if (!ptr)
        rc = vector_set(map->vec, ( uintptr_t )( void * )b, index);
    else
        add_bucket(( struct bucket * )ptr, b);

    if (rc == 0)
        return 0;

    /* Handle error cases */
    if (rc == -1)
        DEBUG("map has reached maximum capacity");
    if (rc == -2)
        DEBUG("index for vector bucket was out of range got %zu max was %zu",
              index, map->vec->capacity);
    if (rc != 1)
        DEBUG("unknown error while setting key %s", key);

    /* rc == 1 -> rehash newly grown vector */
    uintptr_t tmp[map->vec->capacity];
    memset(tmp, 0, map->vec->capacity * sizeof(uintptr_t));

    for (size_t i = 0; i < map->vec->capacity; ++i) {
        if (map->vec->buffer[i] == 0)
            continue;
        struct bucket *bb = ( struct bucket * )map->vec->buffer[i];
        do {
            uint64_t hash  = fnv1a_block_hash(b->key, b->key_len);
            size_t   index = HASH_REDUCE(hash, map->vec);
            b->hash        = index;
            tmp[index]     = ( uintptr_t )bb;
        } while (b->next);
        memmove(map->vec->buffer, tmp, map->vec->capacity * sizeof(uintptr_t));
    }

    return rc;
}

uintptr_t hashmap_get(struct hashmap *map, const unsigned char *key,
                      size_t key_len)
{
    uint64_t  hash  = fnv1a_block_hash(key, key_len);
    size_t    index = HASH_REDUCE(hash, map->vec);
    uintptr_t val   = vector_get(map->vec, index);
    if (val)
        return val;
    return find_bucket(( struct bucket * )val, hash);
}

int hashmap_delete(struct hashmap *map, const unsigned char *key,
                   size_t key_len)
{
    uint64_t  hash  = fnv1a_block_hash(key, key_len);
    size_t    index = HASH_REDUCE(hash, map->vec);
    uintptr_t val   = vector_get(map->vec, index);
    if (val) {
        vector_delete(map->vec, index);
        return 0;
    }
    return delete_bucket(( struct bucket * )val, hash);
}

void hashmap_deinit(struct hashmap *map, int dealloc)
{
    if (dealloc) {
        struct bucket *b = NULL;
        for (size_t i = 0; i < map->vec->capacity; ++i) {
            if (map->vec->buffer[i] == 0)
                continue;
            b = ( struct bucket * )map->vec->buffer[i];
            do {
                struct bucket *tmp = b->next;
                free(b);
                b = tmp;
            } while (b);
        }
    }
    vector_deinit(map->vec, 0); /* Don't need to free values again */
    free(map);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
