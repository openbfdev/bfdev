/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/list.h>
#include <export.h>

export bool
bfdev_list_debug_add_check(struct bfdev_list_head *prev, struct bfdev_list_head *next,
                     struct bfdev_list_head *new)
{
    if (unlikely(prev->next != next)) {
        fprintf(stderr, "list_add corruption (%p) prev->next should be next (%p), but was (%p)\n",
        prev, next, prev->next);
        return false;
    }

    if (unlikely(next->prev != prev)) {
        fprintf(stderr, "list_add corruption (%p) next->prev should be prev (%p), but was (%p)\n",
        next, prev, next->prev);
        return false;
    }

    if (unlikely(new == prev || new == next)) {
        fprintf(stderr, "list_add corruption double add: new=(%p), prev=(%p), next=(%p)\n",
        new, prev, next);
        return false;
    }

    return true;
}

export bool
bfdev_list_debug_del_check(struct bfdev_list_head *node)
{
    if (unlikely(node->next == BFDEV_POISON_LIST1)) {
        printf("list_del corruption (%p) node->next should not be BFDEV_POISON_LIST1 (%p)\n",
        node, BFDEV_POISON_LIST1);
        return false;
    }

    if (unlikely(node->prev == BFDEV_POISON_LIST2)) {
        printf("list_del corruption (%p) node->prev should not be BFDEV_POISON_LIST2 (%p)\n",
        node, BFDEV_POISON_LIST2);
        return false;
    }

    return true;
}
