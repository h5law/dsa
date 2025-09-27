/* $dsa: list.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "macros.h"

int ll_push(struct ll_t *list, struct l_item_t *item)
{
    item->next = NULL;
    if (!list->tail) {
        list->tail = item;
        DEBUG("Adding item to empty list\n")
    } else {
        if (!list->tail->next) {
            list->tail->next = item;
            DEBUG("Adding item to a list with a single item\n");
        } else {
            DEBUG("Finding head of list\n");
            while (list->tail->next) {
                continue;
            }
            list->tail->next = item;
            DEBUG("Adding item to head of the list\n")
        }
    }
    ++list->size;

    return 0;
}

struct l_item_t *ll_peek(struct ll_t *list)
{
    if (!list->tail) {
        return NULL;
        DEBUG("No tail in list\n")
    } else {
        if (!list->tail->next)
            return list->tail;
        else {
            DEBUG("Finding head of list\n");
            while (list->tail->next) {
                continue;
            }
            return list->tail->next;
        }
    }
    return NULL;
}

struct l_item_t *ll_pop(struct ll_t *list)
{
    struct l_item_t *ret = NULL;
    if (!list->tail) {
        return ret;
        DEBUG("No tail in list\n")
    } else {
        if (!list->tail->next) {
            --list->size;
            ret = list->tail;
        } else {
            DEBUG("Finding head of list\n");
            while (list->tail->next) {
                continue;
            }
            ret = list->tail->next;
        }
        --list->size;
    }
    ret->next = NULL;
    return ret;
}

struct l_item_t *ll_find(struct ll_t *list, const uint8_t *data,
                         size_t data_len)
{
    struct l_item_t *b = list->tail;
    if (b == NULL || list->size == 0) {
        DEBUG("Empty list")
        return NULL;
    }
    do {
        if (memcmp(b->data, data, data_len) != 0) {
            DEBUG("Single list entry not a match\n")
            return NULL;
        } else {
            DEBUG("Item found in list")
            return b;
        }
        b = b->next;
    } while (b->next);
    return NULL;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
