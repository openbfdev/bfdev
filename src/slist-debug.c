/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-slist"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/slist.h>
#include <export.h>

export bool
bfdev_slist_check_add(bfdev_slist_head_t *node, bfdev_slist_head_t *newn)
{
    if (bfdev_unlikely(node->next == newn)) {
        bfdev_log_err(
            "slist_add corruption (%p) node->next"
            " should not be newn (%p)\n",
            node, newn
        );
        return false;
    }

    if (bfdev_unlikely(node == newn)) {
        bfdev_log_err(
            "slist_add corruption double add:"
            " newn=(%p), node=(%p)\n",
            newn, node
        );
        return false;
    }

    return true;
}

export bool
bfdev_slist_check_del(bfdev_slist_head_t *node)
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
