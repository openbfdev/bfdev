/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include "list.h"
#include <stdio.h>

bool list_debug_add_check(struct list_head *prev, struct list_head *next, struct list_head *new)
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

bool list_debug_del_check(struct list_head *node)
{
    if (unlikely(node->next == POISON_LIST1)) {
        printf("list_del corruption (%p) node->next should not be POISON_LIST1 (%p)\n",
        node, POISON_LIST1);
        return false;
    }

    if (unlikely(node->prev == POISON_LIST2)) {
        printf("list_del corruption (%p) node->prev should not be POISON_LIST2 (%p)\n",
        node, POISON_LIST2);
        return false;
    }

    return true;
}
