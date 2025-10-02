/* $dsa: demo application of the singly linked-list */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dsa/list.h>

int main(void)
{
    printf("Setting up example environment\n");
    const char *pre_data = "List Item Data";
    const int  pre_data2[4] = {128, 3, 456, 0};
    const char pre_data3[6] = {'H', 'E', 'L', 'L', 'O', '\0'};
    const char *pre_data4 = "Another Item Data";

    size_t data_len = strlen(pre_data);
    size_t data2_len = sizeof(pre_data2) * sizeof(int);
    size_t data3_len = sizeof(pre_data3) * sizeof(char);
    size_t data4_len = strlen(pre_data4);

    const uint8_t *data = calloc(1, data_len);
    const uint8_t *data2 = calloc(1, data2_len);
    const uint8_t *data3 = calloc(1, data3_len);
    const uint8_t *data4 = calloc(1, data4_len);

    data = memcpy(data, pre_data, data_len);
    data2 = memcpy(data2, pre_data2, data2_len);
    data3 = memcpy(data3, pre_data3, data3_len);
    data4 = memcpy(data4, pre_data4, data4_len);

    struct ll_t list = {0};

    struct l_item_t item1 = new_litem(data, data_len);
    struct l_item_t item2 = new_litem(data2, data2_len);
    struct l_item_t item3 = new_litem(data3, data3_len);
    struct l_item_t item4 = new_litem(data4, data4_len);

    struct l_item_t *found = NULL;

    printf("Testing ll_add\n"); /* item1 -> NULL */
    assert(list.size == 0);
    assert(ll_add(&list, &item1) == 0);
    assert(list.size == 1);

    printf("Testing ll_peek\n");
    found = ll_peek(&list);
    assert(memcmp((const uint8_t *)found->data,
                  (const uint8_t *)data, data_len) == 0);
    assert(list.size == 1);

    printf("Testing ll_add_after\n"); /* item1 -> item2 -> NULL */
    assert(ll_add_after(&list, found, &item2) == 0);
    assert(list.size == 2);

    printf("Testing ll_peek\n");
    found = ll_peek(&list);
    assert(memcmp((const uint8_t *)found->data,
                  (const uint8_t *)data2, data2_len) == 0);
    assert(list.size == 2);

    printf("Testing ll_add_before\n"); /* item1 -> item3 -> item2 -> NULL */
    assert(ll_add_before(&list, &item2, &item3) == 0);
    assert(list.size == 3);

    printf("Testing ll_peek\n");
    found = ll_peek(&list);
    assert(memcmp((const uint8_t *)found->data,
                  (const uint8_t *)data2, data2_len) == 0);
    assert(list.size == 3);

    printf("Testing ll_pop\n"); /* item1 -> item3 -> NULL */
    found = ll_pop(&list);
    assert(memcmp((const uint8_t *)found->data,
                  (const uint8_t *)data2, data2_len) == 0);
    assert(list.size == 2);

    printf("Testing ll_add\n"); /* item1 -> item3 -> item4 -> NULL */
    assert(ll_add(&list, &item4) == 0);
    assert(list.size == 3);

    printf("Testing ll_delete_before\n"); /* item1 -> item4 -> NULL */
    assert(ll_delete_before(&list, &item4) == 0);
    assert(list.size = 2);

    printf("Testing ll_peek\n");
    found = ll_peek(&list);
    assert(memcmp((const uint8_t *)found->data,
                  (const uint8_t *)data4, data4_len) == 0);
    assert(list.size == 2);

    printf("Testing ll_add\n") /* item1 -> item4 -> item2 -> NULL */;
    assert(ll_add(&list, &item2) == 0);
    assert(list.size == 3);

    printf("Testing ll_delete_after\n") /* item1 -> item4 -> NULL */;
    assert(ll_delete_after(&list, &item4) == 0);
    assert(list.size = 2);

    printf("Testing ll_peek\n");
    found = ll_peek(&list);
    assert(memcmp((const uint8_t *)found->data,
                  (const uint8_t *)data4, data4_len) == 0);
    assert(list.size == 2);

    free(data);
    free(data2);
    free(data3);
    free(data4);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
