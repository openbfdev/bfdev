/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-slist"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev.h>
#include <bfdev/log.h>
#include <bfdev/slist.h>
#include <export.h>

export bool
bfdev_slist_check_add(struct bfdev_slist_head *node, struct bfdev_slist_head *newn)
{
    if (bfdev_unlikely(newn->next && newn->next == node->next)) {
        bfdev_log_err(
            "bfdev_slist_add corruption (%p) newn->next"
            " should not be next (%p)\n",
            newn, node
        );
        return false;
    }

    return true;
}

export bool
bfdev_slist_check_del(struct bfdev_slist_head *node)
{
    if (bfdev_unlikely(node->next == BFDEV_POISON_SLIST)) {
        bfdev_log_err(
            "bfdev_slist_del corruption (%p) node->next"
            " should not be BFDEV_POISON_SLIST (%p)\n",
            node, BFDEV_POISON_SLIST
        );
        return false;
    }

    return true;
}
