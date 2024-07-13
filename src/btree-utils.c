/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Fredrik Anderson <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/btree.h>
#include <export.h>

#define BLOCK_SIZE 128
#define NODE_SIZE (BLOCK_SIZE - sizeof(bfdev_btree_node_t))
#define UINTPTR_PER_U32 BFDEV_DIV_ROUND_UP(BFDEV_BYTES_PER_U32, BFDEV_BYTES_PER_UINTPTR)
#define UINTPTR_PER_U64 BFDEV_DIV_ROUND_UP(BFDEV_BYTES_PER_U64, BFDEV_BYTES_PER_UINTPTR)

export bfdev_btree_layout_t
bfdev_btree_layout32 = {
    .keylen = UINTPTR_PER_U32,
    .keynum = NODE_SIZE / sizeof(uintptr_t) / (UINTPTR_PER_U32 + 1),
    .ptrindex = UINTPTR_PER_U32 * (NODE_SIZE / sizeof(uintptr_t) / (UINTPTR_PER_U32 + 1)),
    .nodesize = NODE_SIZE,
};

export bfdev_btree_layout_t
bfdev_btree_layout64 = {
    .keylen = UINTPTR_PER_U64,
    .keynum = NODE_SIZE / sizeof(uintptr_t) / (UINTPTR_PER_U64 + 1),
    .ptrindex = UINTPTR_PER_U64 * (NODE_SIZE / sizeof(uintptr_t) / (UINTPTR_PER_U64 + 1)),
    .nodesize = NODE_SIZE,
};

export bfdev_btree_layout_t
bfdev_btree_layoutptr = {
    .keylen = 1,
    .keynum = NODE_SIZE / sizeof(uintptr_t) / 2,
    .ptrindex = NODE_SIZE / sizeof(uintptr_t) / 2,
    .nodesize = NODE_SIZE,
};

export long
bfdev_btree_key_find(bfdev_btree_root_t *root, uintptr_t *node, uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    unsigned int index;

    layout = root->layout;
    for (index = 0; index < layout->keylen; ++index) {
        if (node[index] < key[index])
            return -1;
        if (node[index] > key[index])
            return 1;
    }

    return 0;
}

export void *
bfdev_btree_alloc(bfdev_btree_root_t *root)
{
    const bfdev_alloc_t *alloc;
    bfdev_btree_layout_t *layout;

    alloc = root->alloc;
    layout = root->layout;

    return bfdev_malloc(alloc, layout->nodesize);
}

export void
bfdev_btree_free(bfdev_btree_root_t *root, void *node)
{
    const bfdev_alloc_t *alloc;

    alloc = root->alloc;
    bfdev_free(alloc, node);
}
