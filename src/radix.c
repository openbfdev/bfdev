/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/radix.h>
#include <bfdev/overflow.h>
#include <export.h>

static __bfdev_always_inline unsigned int
radix_depth_shift(unsigned int level)
{
    return BFDEV_RADIX_SHIFT + BFDEV_RADIX_ARY_SHIFT * level;
}

static __bfdev_always_inline unsigned int
radix_depth_index(unsigned int level, uintptr_t offset)
{
    return offset >> radix_depth_shift(level) & BFDEV_RADIX_ARY_MASK;
}

static bfdev_radix_node_t *
radix_parent(bfdev_radix_root_t *root, uintptr_t offset, uintptr_t *index)
{
    unsigned int level = root->level;
    bfdev_radix_node_t *node;

    if (bfdev_ilog2(offset) > radix_depth_shift(level))
        return NULL;

    for (node = root->node; node && level--;) {
        node = node->child[radix_depth_index(level, offset)];
        offset &= BFDEV_BIT_LOW_MASK(radix_depth_shift(level));
    }

    *index = offset;
    return node;
}

export void *
bfdev_radix_root_find(bfdev_radix_root_t *root, uintptr_t offset)
{
    bfdev_radix_node_t *node;
    uintptr_t index;

    node = radix_parent(root, offset, &index);
    if (bfdev_unlikely(!node))
        return NULL;

    return &node->block[index];
}

static inline bfdev_radix_node_t *
radix_extend(bfdev_radix_root_t *root, uintptr_t offset)
{
    const bfdev_alloc_t *alloc = root->alloc;
    bfdev_radix_node_t *node, *successor;
    unsigned int level;

    for (;;) {
        node = root->node;
        level = root->level;

        if (node && bfdev_ilog2(offset) < radix_depth_shift(level))
            break;

        successor = bfdev_zalloc(alloc, sizeof(*successor));
        if (bfdev_unlikely(!successor))
            return NULL;

        if (node) {
            node->parent = successor;
            successor->refcount++;
            root->level++;
        }

        root->node = successor;
        *successor->child = node;
    }

    return node;
}

static inline void
radix_shrink(bfdev_radix_root_t *root)
{
    const bfdev_alloc_t *alloc = root->alloc;
    bfdev_radix_node_t *node, *successor;

    while (root->level) {
        node = root->node;

        if (node->refcount > 1 || !*node->child)
            break;

        successor = *node->child;
        successor->parent = node->parent;
        bfdev_free(alloc, node);

        root->node = successor;
        root->level--;
    }
}

export void *
bfdev_radix_root_alloc(bfdev_radix_root_t *root, uintptr_t offset)
{
    const bfdev_alloc_t *alloc = root->alloc;
    bfdev_radix_node_t *node;
    unsigned int level;

    node = radix_extend(root, offset);
    if (bfdev_unlikely(!node))
        return NULL;

    for (level = root->level; level--;) {
        bfdev_radix_node_t **slot, *newn;

        slot = &node->child[radix_depth_index(level, offset)];
        offset &= BFDEV_BIT_LOW_MASK(radix_depth_shift(level));

        if (!*slot) {
            newn = bfdev_zalloc(alloc, sizeof(*newn));
            if (bfdev_unlikely(!newn))
                return NULL;

            *slot = newn;
            newn->parent = node;
            node->refcount++;
        }

        node = *slot;
    }

    bfdev_bit_set(node->bitmap, offset);
    return &node->block[offset];
}

export int
bfdev_radix_root_free(bfdev_radix_root_t *root, uintptr_t offset)
{
    const bfdev_alloc_t *alloc = root->alloc;
    bfdev_radix_node_t *node;
    unsigned int level;
    uintptr_t index;

    node = radix_parent(root, offset, &index);
    if (bfdev_unlikely(!node))
        return -BFDEV_ENOENT;

    bfdev_bit_clr(node->bitmap, index);
    if (!bfdev_bitmap_empty(node->bitmap, BFDEV_RADIX_BLOCK))
        return -BFDEV_ENOERR;

    for (level = 0; level <= root->level; ++level) {
        bfdev_radix_node_t *parent = node->parent;

        if (level && --node->refcount)
            break;

        offset &= BFDEV_BIT_HIGH_MASK(radix_depth_shift(level));
        if (offset < BFDEV_RADIX_BLOCK)
            break;

        parent->child[radix_depth_index(level, offset)] = NULL;
        bfdev_free(alloc, node);
        node = parent;
    }

    if (level == root->level)
        radix_shrink(root);

    return -BFDEV_ENOERR;
}

export int
bfdev_radix_root_charge(bfdev_radix_root_t *root,
                        uintptr_t offset, size_t size)
{
    uintptr_t end;
    bool retval;

    retval = bfdev_overflow_check_add(offset, size, &end);
    if (bfdev_unlikely(retval))
        return -BFDEV_EOVERFLOW;

    while (offset < end) {
        if (!bfdev_radix_root_alloc(root, offset))
            return -BFDEV_ENOMEM;

        offset += BFDEV_RADIX_BLOCK;
    }

    return -BFDEV_ENOERR;
}

static void
radix_destory_recurse(const bfdev_alloc_t *alloc,
                      bfdev_radix_node_t *node, unsigned int level)
{
    bfdev_radix_node_t *child;
    unsigned int index;

    if (level) {
        for (index = 0; index < BFDEV_RADIX_ARY; ++index) {
            child = node->child[index];
            if (child)
                radix_destory_recurse(alloc, child, level - 1);
        }
    }

    bfdev_free(alloc, node);
}

export void
bfdev_radix_root_destory(bfdev_radix_root_t *root)
{
    const bfdev_alloc_t *alloc = root->alloc;

    radix_destory_recurse(alloc, root->node, root->level);
    root->level = 0;
    root->node = NULL;
}
