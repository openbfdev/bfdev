/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/segtree.h>
#include <export.h>

#define SEGTREE_START(segnode) ((segnode)->start)
#define SEGTREE_END(segnode) ((segnode)->end)
SEGTREE_DEFINE(export, segtree, struct segtree_node, node, unsigned long, subtree, SEGTREE_START, SEGTREE_END);
