/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/slist.h>

bool slist_debug_add_check(struct slist_head *node, struct slist_head *new)
{
    if (unlikely(new->next && new->next == node->next)) {
        fprintf(stderr, "slist_add corruption (%p) new->next should not be next (%p)\n",
            new, node);
        return false;
    }

    return true;
}

bool slist_debug_del_check(struct slist_head *node)
{
    if (unlikely(node->next == POISON_SLIST)) {
        fprintf(stderr, "slist_del corruption (%p) node->next should not be POISON_SLIST (%p)\n",
            node, POISON_SLIST);
        return false;
    }

    return true;
}
