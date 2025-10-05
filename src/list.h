/* $dsa: list.h */

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

struct l_item_t {
    const uint8_t   *data; /* Item data byte stream */
    size_t           size;
    struct l_item_t *next; /* Pointer to next item in list */
};

struct ll_t {
    struct l_item_t *tail; /* Pointer to the beginning of the list */
    size_t           size;
};

static inline struct l_item_t new_litem(const uint8_t *data, size_t len)
{
    return ( struct l_item_t ){
            .data = ( const uint8_t * )data, .size = len, .next = NULL};
}

int ll_add(struct ll_t *list, struct l_item_t *item);
int ll_put(struct ll_t *list, struct l_item_t *item);

int ll_add_after(struct ll_t *list, struct l_item_t *item,
                 struct l_item_t *new_item);
int ll_add_before(struct ll_t *list, struct l_item_t *item,
                  struct l_item_t *new_item);

int ll_delete_after(struct ll_t *list, struct l_item_t *item);
int ll_delete_before(struct ll_t *list, struct l_item_t *item);

struct l_item_t *ll_peek(struct ll_t *list);
struct l_item_t *ll_pop(struct ll_t *list);

struct l_item_t *ll_find(struct ll_t *list, const uint8_t *data,
                         size_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
