/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-list"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev.h>
#include <bfdev/log.h>
#include <bfdev/list.h>
#include <export.h>

export bool
bfdev_list_check_add(struct bfdev_list_head *prev, struct bfdev_list_head *next,
                     struct bfdev_list_head *newn)
{
    if (bfdev_unlikely(prev->next != next)) {
        bfdev_log_err(
            "list_add corruption (%p) prev->next"
            " should be next (%p), but was (%p)\n",
            prev, next, prev->next
        );
        return false;
    }

    if (bfdev_unlikely(next->prev != prev)) {
        bfdev_log_err(
            "list_add corruption (%p) next->prev"
            " should be prev (%p), but was (%p)\n",
            next, prev, next->prev
        );
        return false;
    }

    if (bfdev_unlikely(newn == prev || newn == next)) {
        bfdev_log_err(
            "list_add corruption double add:"
            " newn=(%p), prev=(%p), next=(%p)\n",
            newn, prev, next
        );
        return false;
    }

    return true;
}

export bool
bfdev_list_check_del(struct bfdev_list_head *node)
{
    if (bfdev_unlikely(node->next == BFDEV_POISON_LIST1)) {
        bfdev_log_err(
            "list_del corruption (%p) node->next"
            " should not be BFDEV_POISON_LIST1 (%p)\n",
            node, BFDEV_POISON_LIST1
        );
        return false;
    }

    if (bfdev_unlikely(node->prev == BFDEV_POISON_LIST2)) {
        bfdev_log_err(
            "list_del corruption (%p) node->prev"
            " should not be BFDEV_POISON_LIST2 (%p)\n",
            node, BFDEV_POISON_LIST2
        );
        return false;
    }

    return true;
}
