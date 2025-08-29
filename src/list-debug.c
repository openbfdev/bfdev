/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-list"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/list.h>
#include <export.h>

export bfdev_bool
bfdev_list_check_add(bfdev_list_head_t *prev, bfdev_list_head_t *next,
                     bfdev_list_head_t *newn)
{
    if (bfdev_unlikely(prev->next != next)) {
        bfdev_log_err(
            "insert corruption (%p) prev->next"
            " should be next (%p), but was (%p)\n",
            prev, next, prev->next
        );
        return bfdev_false;
    }

    if (bfdev_unlikely(next->prev != prev)) {
        bfdev_log_err(
            "insert corruption (%p) next->prev"
            " should be prev (%p), but was (%p)\n",
            next, prev, next->prev
        );
        return bfdev_false;
    }

    if (bfdev_unlikely(newn == prev || newn == next)) {
        bfdev_log_err(
            "insert corruption double add:"
            " newn=(%p), prev=(%p), next=(%p)\n",
            newn, prev, next
        );
        return bfdev_false;
    }

    return bfdev_true;
}

export bfdev_bool
bfdev_list_check_del(bfdev_list_head_t *node)
{
    if (bfdev_unlikely(node->next == BFDEV_POISON_LIST1)) {
        bfdev_log_err(
            "delete corruption (%p) node->next"
            " should not be BFDEV_POISON_LIST1 (%p)\n",
            node, BFDEV_POISON_LIST1
        );
        return bfdev_false;
    }

    if (bfdev_unlikely(node->prev == BFDEV_POISON_LIST2)) {
        bfdev_log_err(
            "delete corruption (%p) node->prev"
            " should not be BFDEV_POISON_LIST2 (%p)\n",
            node, BFDEV_POISON_LIST2
        );
        return bfdev_false;
    }

    return bfdev_true;
}
