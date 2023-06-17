/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "slist-debug"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev.h>
#include <bfdev/log.h>
#include <bfdev/slist.h>
#include <export.h>

export bool
bfdev_slist_debug_add_check(struct bfdev_slist_head *node, struct bfdev_slist_head *new)
{
    if (unlikely(new->next && new->next == node->next)) {
        bfdev_log_err(
            "bfdev_slist_add corruption (%p) new->next"
            " should not be next (%p)\n",
            new, node
        );
        return false;
    }

    return true;
}

export bool
bfdev_slist_debug_del_check(struct bfdev_slist_head *node)
{
    if (unlikely(node->next == BFDEV_POISON_SLIST)) {
        bfdev_log_err(
            "bfdev_slist_del corruption (%p) node->next"
            " should not be BFDEV_POISON_SLIST (%p)\n",
            node, BFDEV_POISON_SLIST
        );
        return false;
    }

    return true;
}
