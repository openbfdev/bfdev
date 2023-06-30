/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "list-debug"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev.h>
#include <bfdev/log.h>
#include <bfdev/list.h>
#include <export.h>

static bfdev_log_t log = {
    .log_level = BFDEV_LOG_DEBUG,
    .writer = bfdev_stderr_writer,
    .pdata = NULL
};

export bool
bfdev_list_debug_add_check(struct bfdev_list_head *prev, struct bfdev_list_head *next,
                     struct bfdev_list_head *new)
{
    if (unlikely(prev->next != next)) {
        bfdev_log_err(&log,
            "list_add corruption (%p) prev->next"
            " should be next (%p), but was (%p)\n",
            prev, next, prev->next
        );
        return false;
    }

    if (unlikely(next->prev != prev)) {
        bfdev_log_err(&log,
            "list_add corruption (%p) next->prev"
            " should be prev (%p), but was (%p)\n",
            next, prev, next->prev
        );
        return false;
    }

    if (unlikely(new == prev || new == next)) {
        bfdev_log_err(&log,
            "list_add corruption double add:"
            " new=(%p), prev=(%p), next=(%p)\n",
            new, prev, next
        );
        return false;
    }

    return true;
}

export bool
bfdev_list_debug_del_check(struct bfdev_list_head *node)
{
    if (unlikely(node->next == BFDEV_POISON_LIST1)) {
        bfdev_log_err(&log,
            "list_del corruption (%p) node->next"
            " should not be BFDEV_POISON_LIST1 (%p)\n",
            node, BFDEV_POISON_LIST1
        );
        return false;
    }

    if (unlikely(node->prev == BFDEV_POISON_LIST2)) {
        bfdev_log_err(&log,
            "list_del corruption (%p) node->prev"
            " should not be BFDEV_POISON_LIST2 (%p)\n",
            node, BFDEV_POISON_LIST2
        );
        return false;
    }

    return true;
}
