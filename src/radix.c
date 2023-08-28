/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/radix.h>
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

static struct bfdev_radix_node *
radix_parent(struct bfdev_radix_root *root, uintptr_t offset, uintptr_t *index)
{
    unsigned int level = root->level;
    struct bfdev_radix_node *node;

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
bfdev_radix_root_find(struct bfdev_radix_root *root, uintptr_t offset)
{
    struct bfdev_radix_node *node;
    uintptr_t index;

    node = radix_parent(root, offset, &index);
    if (bfdev_unlikely(!node))
        return NULL;

    return &node->block[index];
}

static inline struct bfdev_radix_node *
radix_extend(struct bfdev_radix_root *root, uintptr_t offset)
{
    const struct bfdev_alloc *alloc = root->alloc;
    struct bfdev_radix_node *node, *successor;
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
radix_shrink(struct bfdev_radix_root *root)
{
    const struct bfdev_alloc *alloc = root->alloc;
    struct bfdev_radix_node *node, *successor;

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
bfdev_radix_root_alloc(struct bfdev_radix_root *root, uintptr_t offset)
{
    const struct bfdev_alloc *alloc = root->alloc;
    struct bfdev_radix_node *node;
    unsigned int level;

    node = radix_extend(root, offset);
    if (bfdev_unlikely(!node))
        return NULL;

    for (level = root->level; level--;) {
	    struct bfdev_radix_node **slot, *new;

        slot = &node->child[radix_depth_index(level, offset)];
        offset &= BFDEV_BIT_LOW_MASK(radix_depth_shift(level));

        if (!*slot) {
            new = bfdev_zalloc(alloc, sizeof(*new));
            if (bfdev_unlikely(!new))
                return NULL;

            *slot = new;
            new->parent = node;
            node->refcount++;
        }

        node = *slot;
    }

    bfdev_bit_set(node->bitmap, offset);
    return &node->block[offset];
}

export int
bfdev_radix_root_free(struct bfdev_radix_root *root, uintptr_t offset)
{
    const struct bfdev_alloc *alloc = root->alloc;
    struct bfdev_radix_node *node;
    unsigned int level;
    uintptr_t index;

    node = radix_parent(root, offset, &index);
    if (bfdev_unlikely(!node))
        return -BFDEV_ENOENT;

    bfdev_bit_clr(node->bitmap, index);
    if (!bfdev_bitmap_empty(node->bitmap, BFDEV_RADIX_BLOCK))
        return -BFDEV_ENOERR;

    for (level = 0; level <= root->level; ++level) {
        struct bfdev_radix_node *parent = node->parent;

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
bfdev_radix_root_charge(struct bfdev_radix_root *root, size_t size)
{
    uintptr_t offset;

	for (offset = 0; offset < size; offset += BFDEV_RADIX_BLOCK) {
		if (!bfdev_radix_root_alloc(root, offset))
			return -BFDEV_ENOMEM;
    }

    return -BFDEV_ENOERR;
}

static void
radix_destory_recurse(const struct bfdev_alloc *alloc,
                      struct bfdev_radix_node *node, unsigned int level)
{
    struct bfdev_radix_node *child;
    unsigned int index;

    if (level) {
        for (index = 0; index < BFDEV_RADIX_ARY; ++index) {
            if ((child = node->child[index]))
                radix_destory_recurse(alloc, child, level - 1);
        }
    }

    bfdev_free(alloc, node);
}

export void
bfdev_radix_root_destory(struct bfdev_radix_root *root)
{
    const struct bfdev_alloc *alloc = root->alloc;

    radix_destory_recurse(alloc, root->node, root->level);
    root->level = 0;
    root->node = NULL;
}
