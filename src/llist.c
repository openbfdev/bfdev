/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/llist.h>
#include <export.h>

export bool
bfdev_llist_split(struct bfdev_slist_head *head, struct bfdev_slist_head *node,
                  struct bfdev_slist_head *end)
{
    struct bfdev_slist_head *first;

    do first = end->next = BFDEV_READ_ONCE(head->next);
    while ((void *)bfdev_cmpxchg(
        (bfdev_atomic_t *)&head->next, (bfdev_atomic_t)first,
        (bfdev_atomic_t)node) != first
    );

    return !first;
}

export struct bfdev_slist_head *
bfdev_llist_del(struct bfdev_slist_head *head)
{
    struct bfdev_slist_head *old, *next, *entry;

    for (entry = BFDEV_READ_ONCE(head->next); (old = entry);) {
        next = BFDEV_READ_ONCE(entry->next);
        entry = (void *)bfdev_cmpxchg(
            (bfdev_atomic_t *)&head->next, (bfdev_atomic_t)old,
            (bfdev_atomic_t)next
        );
        if (old == entry)
            break;
    }

    return entry;
}
