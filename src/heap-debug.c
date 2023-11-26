/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-heap"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/heap.h>
#include <export.h>

export bool
bfdev_heap_check_link(struct bfdev_heap_node *parent, struct bfdev_heap_node **link,
                      struct bfdev_heap_node *node)
{
    if (bfdev_unlikely(*link == node)) {
        bfdev_log_err(
            "insert corruption (%p) *link"
            " should not be node (%p)\n",
            link, node
        );
        return false;
    }

    return true;
}

export bool
bfdev_heap_check_delete(struct bfdev_heap_node *node)
{
    if (bfdev_unlikely(node->left == BFDEV_POISON_HPNODE1)) {
        bfdev_log_err(
            "delete corruption (%p) node->left"
            " should not be BFDEV_POISON_HPNODE1 (%p)\n",
            node, BFDEV_POISON_HPNODE1
        );
        return false;
    }

    if (bfdev_unlikely(node->right == BFDEV_POISON_HPNODE2)) {
        bfdev_log_err(
            "delete corruption (%p) node->right"
            " should not be BFDEV_POISON_HPNODE2 (%p)\n",
            node, BFDEV_POISON_HPNODE2
        );
        return false;
    }

    if (bfdev_unlikely(node->parent == BFDEV_POISON_HPNODE3)) {
        bfdev_log_err(
            "delete corruption (%p) node->parent"
            " should not be BFDEV_POISON_HPNODE3 (%p)\n",
            node, BFDEV_POISON_HPNODE3
        );
        return false;
    }

    return true;
}
