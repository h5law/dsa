/* $dsa: list.h */

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define NEW_LITEM(data, size)                                                  \
    do {                                                                       \
        struct l_item_t item = {0};                                            \
        item.data            = ( const uint8_t * )data;                        \
        item.size            = ( size_t )size;                                 \
        item.next            = NULL;                                           \
        return item;                                                           \
    } while (0)

struct l_item_t {
    uint8_t         *data; /* Item data byte stream */
    size_t           size;
    struct l_item_t *next;
};

struct ll_t {
    struct l_item_t *tail;
    size_t           size;
};

int              ll_push(struct ll_t *list, struct l_item_t *item);
struct l_item_t *ll_peek(struct ll_t *list);
struct l_item_t *ll_pop(struct ll_t *list);
struct l_item_t *ll_find(struct ll_t *list, const uint8_t *data,
                         size_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
