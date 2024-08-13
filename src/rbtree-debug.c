/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-rbtree"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/rbtree.h>
#include <export.h>

export bool
bfdev_rb_check_link(bfdev_rb_node_t *parent, bfdev_rb_node_t **link,
                    bfdev_rb_node_t *node)
{
    if (bfdev_unlikely(*link == node)) {
        bfdev_log_err(
            "rb_insert corruption (%p) *link"
            " should not be node (%p)\n",
            link, node
        );
        return false;
    }

    return true;
}

export bool
bfdev_rb_check_delete(bfdev_rb_node_t *node)
{
    if (bfdev_unlikely(node->left == BFDEV_POISON_RBNODE1)) {
        bfdev_log_err(
            "rb_delete corruption (%p) node->left"
            " should not be BFDEV_POISON_RBNODE1 (%p)\n",
            node, BFDEV_POISON_RBNODE1
        );
        return false;
    }

    if (bfdev_unlikely(node->right == BFDEV_POISON_RBNODE2)) {
        bfdev_log_err(
            "rb_delete corruption (%p) node->right"
            " should not be BFDEV_POISON_RBNODE2 (%p)\n",
            node, BFDEV_POISON_RBNODE2
        );
        return false;
    }

    if (bfdev_unlikely(node->parent == BFDEV_POISON_RBNODE3)) {
        bfdev_log_err(
            "rb_delete corruption (%p) node->parent"
            " should not be BFDEV_POISON_RBNODE3 (%p)\n",
            node, BFDEV_POISON_RBNODE3
        );
        return false;
    }

    return true;
}
