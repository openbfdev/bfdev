/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/list.h>
#include <export.h>

static inline bfdev_list_head_t *
list_merge(bfdev_list_cmp_t cmp, void *pdata,
           bfdev_list_head_t *node1, bfdev_list_head_t *node2)
{
    bfdev_list_head_t *node, **tail;

    tail = &node;
    for (;;) {
        if (cmp(node1, node2, pdata) <= 0) {
            *tail = node1;
            tail = &node1->next;
            node1 = node1->next;
            if (!node1) {
                *tail = node2;
                break;
            }
        } else {
            *tail = node2;
            tail = &node2->next;
            node2 = node2->next;
            if (!node2) {
                *tail = node1;
                break;
            }
        }
    }

    return node;
}

static inline void
list_finish(bfdev_list_cmp_t cmp, void *pdata, bfdev_list_head_t *head,
            bfdev_list_head_t *node1, bfdev_list_head_t *node2)
{
    bfdev_list_head_t *tail;

    tail = head;
    for (;;) {
        if (cmp(node1, node2, pdata) <= 0) {
            tail->next = node1;
            node1->prev = tail;
            tail = node1;
            node1 = node1->next;
            if (!node1)
                break;
        } else {
            tail->next = node2;
            node2->prev = tail;
            tail = node2;
            node2 = node2->next;
            if (!node2) {
                node2 = node1;
                break;
            }
        }
    }

    tail->next = node2;
    do {
        node2->prev = tail;
        tail = node2;
        node2 = node2->next;
    } while (node2);

    tail->next = head;
    head->prev = tail;
}

export void
bfdev_list_sort(bfdev_list_head_t *head, bfdev_list_cmp_t cmp, void *pdata)
{
    bfdev_list_head_t *pending, *node;
    unsigned int count;

    node = head->next;
    if (bfdev_unlikely(node == head->prev))
        return;

    head->prev->next = NULL;
    pending = NULL;

    for (count = 0; node; ++count) {
        bfdev_list_head_t **tail;
        size_t bits;

        tail = &pending;
        for (bits = count; bits & 1; bits >>= 1)
            tail = &(*tail)->prev;

        if (bfdev_likely(bits)) {
            bfdev_list_head_t *node1, *node2;

            node2 = *tail;
            node1 = node2->prev;

            node2 = list_merge(cmp, pdata, node1, node2);
            node2->prev = node1->prev;
            *tail = node2;
        }

        node->prev = pending;
        pending = node;

        node = node->next;
        pending->next = NULL;
    }

    node = pending;
    pending = pending->prev;

    for (;;) {
        bfdev_list_head_t *next;

        next = pending->prev;
        if (!next)
            break;

        node = list_merge(cmp, pdata, pending, node);
        pending = next;
    }

    list_finish(cmp, pdata, head, pending, node);
}
