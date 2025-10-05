/* $dsa: list.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "macros.h"

int ll_add(struct ll_t *list, struct l_item_t *item)
{
    struct l_item_t *b = list->tail;
    item->next = NULL;
    if (list->tail == NULL) {
        DEBUG("Adding item to empty list\n")
        list->tail = item;
        ++list->size;
        return 0;
    } else {
        DEBUG("Finding the head of list\n")
        while (b != NULL) {
            if (b->next == NULL) {
                DEBUG("Adding item to end of list\n")
                b->next = item;
                ++list->size;
                return 0;
            }
            b = b->next;
        }
    }

    DEBUG("Unable to find end of list\n")
    return 1;
}

int ll_put(struct ll_t *list, struct l_item_t *item)
{
    item->next = NULL;
    if (list->tail == NULL) {
        list->tail = item;
        item->next = NULL;
    }
    if (list->tail->next != NULL) {
        item->next = list->tail->next;
        list->tail = item->next;
    }
    ++list->size;

    return 0;
}

int ll_add_after(struct ll_t *list, struct l_item_t *item,
                  struct l_item_t* new)
{
    struct l_item_t *b = list->tail;
    if (list->size == 0 || list->tail == NULL) {
        DEBUG("List is empty, cannot add\n");
        return 1;
    }
    DEBUG("Searching for target item\n");
    do {
        if (memcmp(b->data, item->data, item->size) == 0) {
            DEBUG("Item found, adding after\n");
            new->next = b->next;
            b->next = new;
            ++list->size;
            return 0;
        }
        b = b->next;
    } while (b != NULL);
    return 2;
}

int ll_add_before(struct ll_t *list, struct l_item_t *item,
                   struct l_item_t* new)
{
    struct l_item_t *b = list->tail;
    struct l_item_t *b2 = list->tail;
    if (list->size == 0 || list->tail == NULL) {
        DEBUG("List is empty, cannot remove\n");
        return 1;
    }
    b = b->next;
    DEBUG("Searching for target item\n");
    while (b != NULL) {
        if (memcmp(b->data, item->data, item->size) == 0) {
            DEBUG("Item found adding before\n");
            b2->next = new;
            new->next = b;
            ++list->size;
            return 0;
        }
        b2 = b2->next;
        b = b->next;
    }
    DEBUG("Item not found in list\n");
    return 2;
}

int ll_delete_after(struct ll_t *list, struct l_item_t *item)
{
    struct l_item_t *b = list->tail;
    if (list->size == 0 || list->tail == NULL) {
        DEBUG("Cannot delete from empty list\n")
        return 1;
    }
    while (b != NULL) {
        DEBUG("Searching for target item\n")
        if (memcmp(b->data, item->data, item->size) == 0) {
            DEBUG("Removing item after target item\n")
            b->next = b->next->next;
            --list->size;
            return 0;
        }
        b = b->next;
    }

    return 2;
}

int ll_delete(struct ll_t *list, struct l_item_t *item)
{
    struct l_item_t *b = NULL;
    struct l_item_t *b2 = list->tail;
    struct l_item_t *b3 = b2->next;
    if (list->size == 0 || list->tail == NULL) {
        DEBUG("List is empty\n");
        return 1;
    }
    if (memcmp(list->tail->data, item->data, item->size) == 0) {
        DEBUG("Removing item from list\n")
        struct l_item_t *tmp = list->tail->next;
        list->tail = list->tail->next;
        free(tmp);
        --list->size;
        return 0;
    }
    if (b2 != NULL)
        b3 = b2->next;
    DEBUG("Searching for target item\n")
    while (memcmp(b3->data, item->data, item->size) != 0) {
        if (memcmp(b3->data, item->data, item->size) == 0) {
            DEBUG("Removing item before target item\n")
            if (b2 != NULL)
                b2->next = b3;
            if (b2 == list->tail)
                list->tail = b;
            --list->size;
            return 0;
        }
        b = b2;
        b2 = b3;
        b3 = b3->next;
    }
    return 2;
}

int ll_delete_before(struct ll_t *list, struct l_item_t *item)
{
    struct l_item_t *b = NULL;
    struct l_item_t *b2 = list->tail;
    struct l_item_t *b3 = b2->next;
    if (list->size == 0 || list->tail == NULL) {
        DEBUG("List is empty\n");
        return 1;
    }
    if (memcmp(list->tail->data, item->data, item->size) == 0) {
        DEBUG("Removing list tail\n")
        struct l_item_t *tmp = list->tail->next;
        list->tail = NULL;
        free(tmp);
        --list->size;
        return 0;
    }
    if (b2 != NULL)
        b3 = b2->next;
    DEBUG("Searching for target item\n")
    while (b3 != NULL) {
        if (memcmp(b3->data, item->data, item->size) == 0) {
            DEBUG("Removing item before target item\n")
            if (b != NULL)
                b->next = b3;
            if (b2 == list->tail)
                list->tail = b;
            --list->size;
            return 0;
        }
        b = b2;
        b2 = b3;
        b3 = b3->next;
    }
    return 2;
}

struct l_item_t *ll_peek(struct ll_t *list)
{
    struct l_item_t *b = list->tail;
    if (b == NULL) {
        DEBUG("No items in list\n")
        return NULL;
    } else {
        DEBUG("Finding head of list\n");
        while (b->next != NULL) {
            b = b->next;
        }
        DEBUG("Returning head of list\n");
        return b;
    }
    return NULL;
}

struct l_item_t *ll_pop(struct ll_t *list)
{
    struct l_item_t *b = list->tail;
    struct l_item_t *b2 = list->tail;
    if (list->tail == NULL) {
        DEBUG("No items in list\n")
        return NULL;
    } else {
        b = b->next;
        DEBUG("Finding head of list\n");
        while (b != NULL) {
            b = b->next;
            b2 = b2->next;
        }
        DEBUG("Removing item at head of list\n");
        b2->next = NULL;
        free(b);
        --list->size;
        DEBUG("Returning head of list\n");
        return b2;
    }
    return NULL;
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
