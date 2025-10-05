/* $dsa: map.c */

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "map.h"
#include "macros.h"

/* FNV-1a standard initial prime offset */
#define HASH_VALUE_OFFSET 0XCBF29CE484222325
/* SplitMix64 highly dispersive odd u64 */
#define HASH_VALUE_PRIME  0XD6E8FEB86659FD93

uint64_t fnv1a_block_hash(const unsigned char *data, size_t len)
{
    DEBUG("Process data blocks FNV-1a + SplitMix64 hasher\n")
    size_t   blocks = len / 8;
    uint64_t hash   = HASH_VALUE_OFFSET;
    for (size_t i = 0; i < blocks; ++i) {
        hash ^= *( const uint64_t * )data;
        hash *= HASH_VALUE_PRIME;
        data += 8;
    }

    /* Process any left-overs bytes from the block processor */
    uint64_t last = len & 0xFF;
    if (last > 0) {
        DEBUG("Hashing final parts of block split\n")
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
    }

    return hash;
}

static inline void           add_bucket(struct bucket *b1, struct bucket *b2);
static inline struct bucket *find_bucket(struct vector *, uint64_t hash);

void add_bucket(struct bucket *b1, struct bucket *b2)
{
    DEBUG("Finding the head of the list")
    struct bucket *b = b1->next;
    if (b != NULL)
        b->next = b2;
    while (b->next != NULL) {
        b = b->next;
    }
    DEBUG("Adding bucket to end of list")
    b->next = b2;
}

static inline struct bucket *find_bucket(struct vector *vec, uint64_t hash)
{
    size_t         index = 0;
    struct bucket *val   = NULL;

    index                = HASH_REDUCE(hash, vec);
    val                  = ( struct bucket * )vec->buffer[index];

    DEBUG("Finding the matching item in the list")
    while (val != NULL) {
        if (hash == val->hash) {
            return val;
        }
        val = val->next;
    }
    if (val == NULL)
        DEBUG("Unable to find target item\n")
    return NULL;
}

static inline int delete_bucket(struct vector *vec, uint64_t hash)
{
    size_t         index = 0;
    struct bucket *val   = NULL;

    index                = HASH_REDUCE(hash, vec);
    val                  = ( struct bucket * )vec->buffer[index];

    DEBUG("Removing the matching item in the map")
    while (val != NULL) {
        if (hash == val->hash) {
            val->next = val->next->next;
            free(val);
            return 0;
        }
        val = val->next;
    }
    if (val == NULL)
        DEBUG("Unable to find target item\n")
    return 1;
}
struct bucket new_item(const void *key, size_t key_len, uintptr_t value)
{
    return ( struct bucket ){.key     = key,
                             .key_len = key_len,
                             .next    = NULL,
                             .hash    = 0,
                             .value   = value};
}

int hashmap_set(struct hashmap *map, const uint8_t *key, size_t key_len,
                uintptr_t value)
{
    int           rc    = 0;
    uint64_t      hash  = 0;
    size_t        index = 0;
    uintptr_t     ptr   = 0;
    struct bucket b     = {0};

    index               = HASH_REDUCE(hash, map->vec);
    hash                = fnv1a_block_hash(key, key_len);
    b                   = new_item(key, key_len, value);

    DEBUG("Value inserts at index %zu\n", index);
    ptr = vector_get(map->vec, index);
    if (ptr != 0) {
        rc = vector_set(map->vec, ( uintptr_t )&b, index);
    } else {
        add_bucket(( struct bucket * )ptr, &b);
    }

    if (rc == 0)
        return 0;

    /* Handle error cases */
    if (rc == -1)
        DEBUG("map has reached maximum capacity");
    if (rc == -2)
        DEBUG("index for vector bucket was out of range got %zu max was "
              "%zu",
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
            uint64_t hash  = fnv1a_block_hash(b.key, b.key_len);
            size_t   index = HASH_REDUCE(hash, map->vec);
            b.hash         = index;
            tmp[index]     = ( uintptr_t )bb;
        } while (b.next);
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
    if (val != 0)
        return ( uintptr_t )find_bucket(map->vec, hash);
    else
        return map->vec->buffer[index];
}

int hashmap_delete(struct hashmap *map, const unsigned char *key,
                   size_t key_len)
{
    uint64_t  hash  = fnv1a_block_hash(key, key_len);
    size_t    index = HASH_REDUCE(hash, map->vec);
    uintptr_t val   = 0;

    if (map->size == 0) {
        DEBUG("Empty list\n")
        return 1;
    }

    val = vector_get(map->vec, index);
    if (val == 0) {
        DEBUG("Key not found\n")
        return 2;
    }
    if (val > 0) {
        if (delete_bucket(map->vec, hash) != 0) {
            DEBUG("Key not found\n")
            return 3;
        }
        --map->size;
        --map->vec->count;
        return 0;
    }
    return 4;
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
                if (b != NULL)
                    free(b);
                b = tmp;
            } while (b);
        }
    }
    vector_deinit(map->vec, 0); /* Don't need to free values again */
    free(map);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
