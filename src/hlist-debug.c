/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-hlist"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/hlist.h>
#include <export.h>

export bool
bfdev_hlist_check_head_add(struct bfdev_hlist_head *head,
                           struct bfdev_hlist_node *newn)
{
    if (bfdev_unlikely(head->node == newn)) {
        bfdev_log_err(
            "bfdev_hlist_head_add corruption (%p) head->node"
            " should not be new (%p)\n", head, newn
        );
        return false;
    }

    return true;
}

export bool
bfdev_hlist_check_next_add(struct bfdev_hlist_node *next,
                           struct bfdev_hlist_node *newn)
{
    if (bfdev_unlikely(next->next == newn)) {
        bfdev_log_err(
            "bfdev_hlist_next_add corruption (%p) next->next"
            " should not be new (%p)\n",
            next, newn
        );
        return false;
    }

    return true;
}

export bool
bfdev_hlist_check_prev_add(struct bfdev_hlist_node *prev,
                           struct bfdev_hlist_node *newn)
{
    if (bfdev_unlikely(prev->pprev == &newn->next)) {
        bfdev_log_err(
            "bfdev_hlist_prev_add corruption (%p) prev->pprev"
            " should not be new (%p)\n",
            prev, newn
        );
        return false;
    }

    return true;
}

export bool
bfdev_hlist_check_del(struct bfdev_hlist_node *node)
{
    if (bfdev_unlikely(node->next == BFDEV_POISON_HLIST1)) {
        bfdev_log_err(
            "bfdev_hlist_del corruption (%p) node->next"
            " should not be BFDEV_POISON_HLIST1 (%p)\n",
            node, BFDEV_POISON_HLIST1
        );
        return false;
    }

    if (bfdev_unlikely(node->pprev == BFDEV_POISON_HLIST2)) {
        bfdev_log_err(
            "bfdev_hlist_del corruption (%p) node->pprev"
            " should not be BFDEV_POISON_HLIST2 (%p)\n",
            node, BFDEV_POISON_HLIST2
        );
        return false;
    }

    return true;
}
