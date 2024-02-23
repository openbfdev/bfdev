/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/llist.h>
#include <export.h>

export bool
bfdev_llist_split(bfdev_slist_head_t *head, bfdev_slist_head_t *node,
                  bfdev_slist_head_t *end)
{
    bfdev_slist_head_t *entry;

    entry = BFDEV_READ_ONCE(head->next);
    do {
        end->next = entry;
    } while (!bfdev_try_cmpxchg(
        (bfdev_atomic_t *)&head->next, (bfdev_atomic_t *)&entry,
        (bfdev_atomic_t)node)
    );

    return !entry;
}

export bfdev_slist_head_t *
bfdev_llist_del(bfdev_slist_head_t *head)
{
    bfdev_slist_head_t *next, *entry;

    entry = BFDEV_READ_ONCE(head->next);
    do {
        if (!entry)
            return NULL;
        next = BFDEV_READ_ONCE(entry->next);
    } while (!bfdev_try_cmpxchg(
        (bfdev_atomic_t *)&head->next, (bfdev_atomic_t *)&entry,
        (bfdev_atomic_t)next)
    );

    return entry;
}
