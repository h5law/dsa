/* $dsa: demo application of the singly linked-list */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <dsa/list.h>

int main(void)
{
    const char *data = "List Item Data";
    size_t size = strlen(data);

    struct ll_t list = {0};
    struct l_item_t *item1 = NULL;
    struct l_item_t *item2 = NULL;

    struct l_item_t item = NEW_LITEM(data, size);

    assert(list.size == 0);
    ll_push(&list, &item);
    assert(list.size == 1);

    item1 = ll_peek(&list);
    assert(item1 != NULL && list.size == 1);
    assert(memcmp(item1->data, data, size) == 0);

    item2 = ll_pop(&list);
    assert(item2 != NULL && list.size == 0);
    assert(memcmp(item2->data, data, size) == 0);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
