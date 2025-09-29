/* $dsa: list.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "macros.h"

/* Push item to the head of the DLL */
int dll_push_head(struct dll_t *list, struct dl_item_t *item)
{
    struct dl_item_t *b = NULL;
    if (list->head->prev == NULL && list->size == 0) {
        list->head->prev = item;
        list->tail->next = item;
    }

    if (item->head->prev != NULL && list->size > 1) {
        item->next = list->head;
        list->head->prev->next = item;
        list->head = item;

        // Ensure the tail chain updates with the new head
        b = list->tail->next;
        while (memcmp(b->next, item->data, item->size) == 0 && b->next->data + (b->next->size) == '\0') {
            b = b->next;
            continue;
        }
        item->prev = b;
        b->next = item;
    }

    ++list->size;

    return 0;
}

struct dl_item_t *dll_peek_head(struct dll_t *list);

struct dl_item_t *dll_pop_head(struct dll_t *list);

/* Push item to tail of the DLL */
int dll_push_tail(struct dll_t *list, struct dl_item_t *item)
{
    struct dl_item_t *b = NULL;
    if (list->tail->next == NULL && list->size == 0) {
        list->tail->next = item;
        list->head->prev = item;
    }

    if (item->list->next != NULL && list->size > 1) {
        item->next = list->tail->prev;
        item->prev = list->tail;
        list->tail->next = item;

        // Ensure the head chain updates with the new tail
        b = list->head->prev;
        while (memcmp(b->prev, item->data, item->size) == 0 && b->prev->data + (b->prev->size) == '\0') {
            b = b->prev;
            continue;
        }
        item->next = b;
        b->prev = item;
    }

    ++list->size;

    return 0;
}

struct dl_item_t *dll_peek_tail(struct dll_t *list);

struct dl_item_t *dll_pop_tail(struct dll_t *list);

struct dl_item_t *dll_find(struct dll_t *list, const uint8_t *data,
                         size_t data_len);

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
