/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/segtree.h>
#include <export.h>

#define SEGTREE_START(segnode) ((segnode)->start)
#define SEGTREE_END(segnode) ((segnode)->end)

BFDEV_SEGTREE_DEFINE(
    export, bfdev_segtree, bfdev_segtree_node_t,
    node, unsigned long, subtree, SEGTREE_START, SEGTREE_END
);
