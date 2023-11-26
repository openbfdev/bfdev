/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/list.h>
#include <export.h>

static inline struct bfdev_list_head *
list_merge(bfdev_list_cmp_t cmp, void *pdata,
           struct bfdev_list_head *a, struct bfdev_list_head *b)
{
    struct bfdev_list_head *node, **tail = &node;

    for (;;) {
        if (cmp(a, b, pdata) <= 0) {
            *tail = a;
            tail = &a->next;
            a = a->next;
            if (!a) {
                *tail = b;
                break;
            }
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
            if (!b) {
                *tail = a;
                break;
            }
        }
    }

    return node;
}

static inline void
list_finish(bfdev_list_cmp_t cmp, void *pdata, struct bfdev_list_head *head,
            struct bfdev_list_head *a, struct bfdev_list_head *b)
{
    struct bfdev_list_head *tail = head;
    unsigned int count = 0;

    for (;;) {
        if (cmp(a, b, pdata) <= 0) {
            tail->next = a;
            a->prev = tail;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            b->prev = tail;
            tail = b;
            b = b->next;
            if (!b) {
                b = a;
                break;
            }
        }
    }

    tail->next = b;
    do {
        if (bfdev_unlikely(!++count))
            cmp(b, b, pdata);
        b->prev = tail;
        tail = b;
        b = b->next;
    } while (b);

    tail->next = head;
    head->prev = tail;
}

export void
bfdev_list_sort(struct bfdev_list_head *head, bfdev_list_cmp_t cmp, void *pdata)
{
    struct bfdev_list_head *pending, *node;
    unsigned int count;

    node = head->next;
    if (bfdev_unlikely(node == head->prev))
        return;

    head->prev->next = NULL;
    pending = NULL;

    for (count = 0; node; ++count) {
        struct bfdev_list_head **tail = &pending;
        size_t bits;

        for (bits = count; bits & 1; bits >>= 1)
            tail = &(*tail)->prev;

        if (bfdev_likely(bits)) {
            struct bfdev_list_head *b = *tail, *a = b->prev;
            b = list_merge(cmp, pdata, a, b);
            b->prev = a->prev;
            *tail = b;
        }

        node->prev = pending;
        pending = node;

        node = node->next;
        pending->next = NULL;
    }

    node = pending;
    pending = pending->prev;

    for (;;) {
        struct bfdev_list_head *next = pending->prev;

        if (!next)
            break;
        node = list_merge(cmp, pdata, pending, node);
        pending = next;
    }

    list_finish(cmp, pdata, head, pending, node);
}
