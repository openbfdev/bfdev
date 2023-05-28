/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/hlist.h>
#include <export.h>

export bool
hlist_debug_head_add_check(struct hlist_head *head, struct hlist_node *new)
{
    if (unlikely(head->node == new)) {
        fprintf(stderr, "hlist_head_add corruption (%p) head->node should not be new (%p)\n",
            head, new);
        return false;
    }

    return true;
}

export bool
hlist_debug_next_add_check(struct hlist_node *next, struct hlist_node *new)
{
    if (unlikely(next->next == new)) {
        fprintf(stderr, "hlist_next_add corruption (%p) next->next should not be new (%p)\n",
            next, new);
        return false;
    }

    return true;
}

export bool
hlist_debug_prev_add_check(struct hlist_node *prev, struct hlist_node *new)
{
    if (unlikely(prev->pprev == &new->next)) {
        fprintf(stderr, "hlist_prev_add corruption (%p) prev->pprev should not be new (%p)\n",
            prev, new);
        return false;
    }

    return true;
}

export bool
hlist_debug_del_check(struct hlist_node *node)
{
    if (unlikely(node->next == BFDEV_POISON_HLIST1)) {
        fprintf(stderr, "hlist_del corruption (%p) node->next should not be BFDEV_POISON_HLIST1 (%p)",
            node, BFDEV_POISON_HLIST1);
        return false;
    }

    if (unlikely(node->pprev == BFDEV_POISON_HLIST2)) {
        fprintf(stderr, "hlist_del corruption (%p) node->pprev should not be BFDEV_POISON_HLIST2 (%p)",
            node, BFDEV_POISON_HLIST2);
        return false;
    }

    return true;
}
