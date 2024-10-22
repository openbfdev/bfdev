/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/radix.h>
#include <bfdev/overflow.h>
#include <export.h>

#define RADIX_BLOCK_MASK (BFDEV_RADIX_BLOCK - 1)
#define RADIX_ARY_MASK (BFDEV_RADIX_ARY - 1)

#define RADIX_ARY_SHIFT bfdev_ilog2(BFDEV_RADIX_ARY)
#define RADIX_LEVEL_MAX (BFDEV_BITS_PER_LONG / RADIX_ARY_SHIFT)

struct radix_parent {
    bfdev_radix_node_t *node;
    unsigned int index;
};

static __bfdev_always_inline unsigned int
radix_depth_shift(unsigned int level)
{
    return RADIX_ARY_SHIFT * level + BFDEV_RADIX_SHIFT;
}

static __bfdev_always_inline unsigned int
radix_depth_index(unsigned int level, uintptr_t offset)
{
    return (offset >> radix_depth_shift(level)) & RADIX_ARY_MASK;
}

static bool
radix_parent(bfdev_radix_root_t *root, uintptr_t offset,
             struct radix_parent *parent)
{
    bfdev_radix_node_t *node;
    unsigned int level, index;

    level = root->level;
    BFDEV_BUG_ON(level > RADIX_LEVEL_MAX);

    /* Directly check capacity overflow */
    if (bfdev_ilog2(offset) > radix_depth_shift(level))
        return false;

    node = root->node;
    parent[root->level].node = node;

    if (bfdev_unlikely(!node))
        return false;

    while (level--) {
        index = radix_depth_index(level, offset);
        node = node->child[index];
        offset &= BFDEV_BIT_LOW_MASK(radix_depth_shift(level));

        if (bfdev_unlikely(!node))
            return false;

        parent[level].node = node;
        parent[level + 1].index = index;
    }

    parent[0].index = offset;
    return true;
}

export void *
bfdev_radix_root_find(bfdev_radix_root_t *root, uintptr_t offset)
{
    struct radix_parent parents[RADIX_LEVEL_MAX];
    bfdev_radix_node_t *node;
    unsigned int index;
    bool contain;

    contain = radix_parent(root, offset, parents);
    if (bfdev_unlikely(!contain))
        return NULL;

    node = parents[0].node;
    index = parents[0].index;

    contain = bfdev_bit_test(node->bitmap, index);
    if (bfdev_unlikely(!contain))
        return NULL;

    return &node->block[index];
}

static inline bfdev_radix_node_t *
radix_extend(bfdev_radix_root_t *root, uintptr_t offset)
{
    const bfdev_alloc_t *alloc;
    bfdev_radix_node_t *node, *successor;
    unsigned int level;

    alloc = root->alloc;
    for (;;) {
        node = root->node;
        level = root->level;

        if (node && bfdev_ilog2(offset) < radix_depth_shift(level))
            break;

        successor = bfdev_zalloc(alloc, sizeof(*successor));
        if (bfdev_unlikely(!successor))
            return NULL;

        if (node) {
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
    const bfdev_alloc_t *alloc;
    bfdev_radix_node_t *node, *successor;

    alloc = root->alloc;
    while (root->level) {
        node = root->node;

        if (node->refcount > 1 || !*node->child)
            break;

        successor = *node->child;
        bfdev_free(alloc, node);

        root->node = successor;
        root->level--;
    }
}

export void *
bfdev_radix_root_alloc(bfdev_radix_root_t *root, uintptr_t offset)
{
    const bfdev_alloc_t *alloc;
    bfdev_radix_node_t *node;
    unsigned int level;

    node = radix_extend(root, offset);
    if (bfdev_unlikely(!node))
        return NULL;

    alloc = root->alloc;
    for (level = root->level; level--;) {
        bfdev_radix_node_t **slot, *newn;

        slot = &node->child[radix_depth_index(level, offset)];
        offset &= BFDEV_BIT_LOW_MASK(radix_depth_shift(level));

        if (!*slot) {
            newn = bfdev_zalloc(alloc, sizeof(*newn));
            if (bfdev_unlikely(!newn))
                return NULL;

            *slot = newn;
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
    struct radix_parent parents[RADIX_LEVEL_MAX];
    const bfdev_alloc_t *alloc;
    bfdev_radix_node_t *node;
    unsigned int level, index;
    bool contain;

    contain = radix_parent(root, offset, parents);
    if (bfdev_unlikely(!contain))
        return -BFDEV_ENOENT;

    node = parents[0].node;
    index = parents[0].index;

    bfdev_bit_clr(node->bitmap, index);
    if (!bfdev_bitmap_empty(node->bitmap, BFDEV_RADIX_BLOCK))
        return -BFDEV_ENOERR;

    alloc = root->alloc;
    for (level = 0; level <= root->level; ++level) {
        bfdev_radix_node_t *parent;

        if (level && --node->refcount)
            break;

        /* Do not prune the left most branch */
        offset &= BFDEV_BIT_HIGH_MASK(radix_depth_shift(level));
        if (offset < BFDEV_RADIX_BLOCK)
            break;

        parent = parents[level + 1].node;
        index = parents[level + 1].index;

        parent->child[index] = NULL;
        bfdev_free(alloc, node);
        node = parent;
    }

    /* Prune the leftmost branch */
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
radix_destroy_recurse(const bfdev_alloc_t *alloc,
                      bfdev_radix_node_t *node, unsigned int level)
{
    bfdev_radix_node_t *child;
    unsigned int index;

    if (level) {
        for (index = 0; index < BFDEV_RADIX_ARY; ++index) {
            child = node->child[index];
            if (child)
                radix_destroy_recurse(alloc, child, level - 1);
        }
    }

    bfdev_free(alloc, node);
}

export void
bfdev_radix_root_release(bfdev_radix_root_t *root)
{
    const bfdev_alloc_t *alloc;

    alloc = root->alloc;
    radix_destroy_recurse(alloc, root->node, root->level);

    root->level = 0;
    root->node = NULL;
}

static inline bfdev_radix_node_t *
radix_left_most(bfdev_radix_node_t *node, unsigned int level,
                uintptr_t *offset)
{
    unsigned int walk;

    while (level--) {
        for (walk = 0; walk < BFDEV_RADIX_ARY; ++walk) {
            if (!node->child[walk])
                continue;

            node = node->child[walk];
            *offset |= (uintptr_t)walk << radix_depth_shift(level);
            break;
        }

        if (walk == BFDEV_RADIX_ARY)
            return NULL;
    }

    return node;
}

static inline bfdev_radix_node_t *
radix_right_most(bfdev_radix_node_t *node, unsigned int level,
                 uintptr_t *offset)
{
    unsigned int walk;

    while (level--) {
        walk = BFDEV_RADIX_ARY;
        while (walk--) {
            if (!node->child[walk])
                continue;

            node = node->child[walk];
            *offset |= (uintptr_t)walk << radix_depth_shift(level);
            break;
        }

        if (walk > BFDEV_RADIX_ARY)
            return NULL;
    }

    return node;
}

export void *
bfdev_radix_root_first(bfdev_radix_root_t *root, uintptr_t *offsetp)
{
    unsigned int count, level;
    bfdev_radix_node_t *node;
    uintptr_t offset;

    node = root->node;
    level = root->level;

    if (!node)
        return NULL;

    offset = 0;
    node = radix_left_most(node, level, &offset);
    BFDEV_BUG_ON(!node);

    count = bfdev_find_first_bit(node->bitmap, BFDEV_RADIX_BLOCK);
    if (count == BFDEV_RADIX_BLOCK)
        return NULL;

    offset |= count;
    *offsetp = offset;

    return &node->block[count];
}

export void *
bfdev_radix_root_last(bfdev_radix_root_t *root, uintptr_t *offsetp)
{
    unsigned int count, level;
    bfdev_radix_node_t *node;
    uintptr_t offset;

    node = root->node;
    level = root->level;

    if (!node)
        return NULL;

    offset = 0;
    node = radix_right_most(node, level, &offset);
    BFDEV_BUG_ON(!node);

    count = bfdev_find_last_bit(node->bitmap, BFDEV_RADIX_BLOCK);
    if (count == BFDEV_RADIX_BLOCK)
        return NULL;

    offset |= count;
    *offsetp = offset;

    return &node->block[count];
}

export void *
bfdev_radix_root_next(bfdev_radix_root_t *root, uintptr_t *offsetp)
{
    struct radix_parent parents[RADIX_LEVEL_MAX];
    bfdev_radix_node_t *node;
    unsigned int count, index, level;
    bool contain;

    contain = radix_parent(root, *offsetp, parents);
    if (bfdev_unlikely(!contain))
        return NULL;

    node = parents[0].node;
    index = parents[0].index;

    /* Check for safety */
    contain = bfdev_bit_test(node->bitmap, index);
    if (bfdev_unlikely(!contain))
        return NULL;

    count = bfdev_find_next_bit(node->bitmap, BFDEV_RADIX_BLOCK, index + 1);
    if (count < BFDEV_RADIX_BLOCK)
        goto finish;

    for (level = 1; level <= root->level; ++level) {
        node = parents[level].node;
        index = parents[level].index;

        while (++index < BFDEV_RADIX_ARY) {
            if (!node->child[index])
                continue;

            count = radix_depth_shift(level - 1);
            *offsetp &= ~((uintptr_t)RADIX_ARY_MASK << count);
            *offsetp |= (uintptr_t)index << count;

            node = node->child[index];
            goto downward;
        }
    }

    return NULL;

downward:
    node = radix_left_most(node, level - 1, offsetp);
    BFDEV_BUG_ON(!node);
    count = bfdev_find_first_bit(node->bitmap, BFDEV_RADIX_BLOCK);

finish:
    *offsetp &= ~(uintptr_t)RADIX_BLOCK_MASK;
    *offsetp |= count;

    return &node->block[count];
}

export void *
bfdev_radix_root_prev(bfdev_radix_root_t *root, uintptr_t *offsetp)
{
    struct radix_parent parents[RADIX_LEVEL_MAX];
    bfdev_radix_node_t *node;
    unsigned int count, index, level;
    bool contain;

    contain = radix_parent(root, *offsetp, parents);
    if (bfdev_unlikely(!contain))
        return NULL;

    node = parents[0].node;
    index = parents[0].index;

    /* Check for safety */
    contain = bfdev_bit_test(node->bitmap, index);
    if (bfdev_unlikely(!contain))
        return NULL;

    count = bfdev_find_prev_bit(node->bitmap, BFDEV_RADIX_BLOCK, index - 1);
    if (count < BFDEV_RADIX_BLOCK)
        goto finish;

    for (level = 1; level <= root->level; ++level) {
        node = parents[level].node;
        index = parents[level].index;

        while (index--) {
            if (!node->child[index])
                continue;

            count = radix_depth_shift(level - 1);
            *offsetp &= ~((uintptr_t)RADIX_ARY_MASK << count);
            *offsetp |= (uintptr_t)index << count;

            node = node->child[index];
            goto downward;
        }
    }

    return NULL;

downward:
    node = radix_right_most(node, level - 1, offsetp);
    BFDEV_BUG_ON(!node);
    count = bfdev_find_last_bit(node->bitmap, BFDEV_RADIX_BLOCK);

finish:
    *offsetp &= ~(uintptr_t)RADIX_BLOCK_MASK;
    *offsetp |= count;

    return &node->block[count];
}
