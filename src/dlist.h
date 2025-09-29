/* $dsa: list.h */

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define NEW_DLITEM(data, size)                                                 \
    do {                                                                       \
        struct l_item_t item = {0};                                            \
        item.data            = ( const uint8_t * )data;                        \
        item.size            = ( size_t )size;                                 \
        item.next            = NULL;                                           \
        item.prev            = NULL;                                           \
        return item;                                                           \
    } while (0)

struct dl_item_t {
    uint8_t         *data; /* Item data byte stream */
    size_t           size;
    struct l_item_t *next;
    struct l_item_t *prev;
};

struct dll_t {
    struct l_item_t *tail;
    struct l_item_t *head;
    size_t           size;
};

int              dll_push_head(struct dll_t *list, struct dl_item_t *item);
struct dl_item_t *dll_peek_head(struct dll_t *list);
struct dl_item_t *dll_pop_head(struct dll_t *list);

int              dll_push_tail(struct dll_t *list, struct dl_item_t *item);
struct dl_item_t *dll_peek_tail(struct dll_t *list);
struct dl_item_t *dll_pop_tail(struct dll_t *list);

struct dl_item_t *dll_find(struct dll_t *list, const uint8_t *data,
                         size_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
