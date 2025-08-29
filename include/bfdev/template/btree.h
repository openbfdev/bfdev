/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TEMPLATE_BTREE_H_
#define _BFDEV_TEMPLATE_BTREE_H_

#include <bfdev/config.h>
#include <bfdev/btree.h>

BFDEV_BEGIN_DECLS

extern bfdev_btree_layout_t
bfdev_btree_layout32;

extern bfdev_btree_layout_t
bfdev_btree_layout64;

extern bfdev_btree_layout_t
bfdev_btree_layoutptr;

extern long
bfdev_btree_key_find(bfdev_btree_root_t *root, bfdev_uintptr_t *node,
                     bfdev_uintptr_t *key);

extern void *
bfdev_btree_alloc(bfdev_btree_root_t *root);

extern void
bfdev_btree_free(bfdev_btree_root_t *root, void *node);

BFDEV_END_DECLS

#endif /* _BFDEV_TEMPLATE_BTREE_H_ */
