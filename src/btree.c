/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/btree.h>
#include <export.h>

static __bfdev_always_inline void *
bnode_get_value(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                unsigned int index)
{
    struct bfdev_btree_layout *layout = root->layout;
    return (void *)node->block[layout->ptrindex + index];
}

static __bfdev_always_inline void
bnode_set_value(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                unsigned int index, void *value)
{
    struct bfdev_btree_layout *layout = root->layout;
    node->block[layout->ptrindex + index] = (uintptr_t)value;
}

static __bfdev_always_inline uintptr_t *
bnode_get_key(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
              unsigned int index)
{
    struct bfdev_btree_layout *layout = root->layout;
    return &node->block[layout->keylen * index];
}

static __bfdev_always_inline void
bnode_set_key(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
              unsigned int index, uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    memcpy(bnode_get_key(root, node, index), key,
           layout->keylen * sizeof(uintptr_t));
}

static inline void
bnode_takeout_key(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                  unsigned int index, uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    memcpy(key, bnode_get_key(root, node, index),
           layout->keylen * sizeof(uintptr_t));
}

static inline long
bnode_cmp_key(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
              unsigned int index, uintptr_t *key)
{
    return root->find(root, bnode_get_key(root, node, index), key);
}

static inline struct bfdev_btree_node *
bnode_alloc(struct bfdev_btree_root *root)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node;

    node = root->alloc(root);
    if (likely(node))
        memset(node, 0, layout->nodesize);

    return node;
}

static inline void
bnode_free(struct bfdev_btree_root *root, struct bfdev_btree_node *node)
{
    return root->free(root, node);
}

static inline void
bnode_clear_index(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                  unsigned int index)
{
    struct bfdev_btree_layout *layout = root->layout;
    bnode_set_value(root, node, index, NULL);
    memset(bnode_get_key(root, node, index), 0,
           layout->keylen * sizeof(uintptr_t));
}

static inline bool
btree_empty_key(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    unsigned int count;

    for (count = 0; count < layout->keylen; ++count) {
        if (key[count])
            return false;
    }

    return true;
}

static unsigned int
bnode_fill_index(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                 unsigned int start)
{
    struct bfdev_btree_layout *layout = root->layout;
    unsigned int index;

    for (index = start; index < layout->keynum; ++index) {
        if (!bnode_get_value(root, node, index))
            break;
    }

    return index;
}

static unsigned int
bnode_key_index(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    unsigned int index;
    long retval;

    for (index = 0; index < layout->keynum; ++index) {
        retval = bnode_cmp_key(root, node, index, key);
        if (retval < 0)
            break;
        else if (retval == 0)
            return index;
    }

    return layout->keynum;
}

static unsigned int
bnode_find_index(struct bfdev_btree_root *root, struct bfdev_btree_node *node,
                 uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    unsigned int index;

    for (index = 0; index < layout->keynum; ++index) {
        if (bnode_cmp_key(root, node, index, key) <= 0)
            break;
    }

    return index;
}

static struct bfdev_btree_node *
bnode_find_parent(struct bfdev_btree_root *root, uintptr_t *key,
                  unsigned int level)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node = root->node;
    unsigned int height, index;

    for (height = level; height < root->height; ++height) {
        for (index = 0; index < layout->keynum; ++index) {
            if (bnode_cmp_key(root, node, index, key) <= 0)
                break;
        }

        if (index == layout->keynum || !bnode_get_value(root, node, index))
            bnode_set_key(root, node, --index, key);

        node = bnode_get_value(root, node, index);
    }

    return node;
}

static struct bfdev_btree_node *
bnode_lookup(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node = root->node;
    unsigned int height = root->height, index;

    if (!height)
        return NULL;

    while (--height) {
        for (index = 0; index < layout->keynum; ++index) {
            if (bnode_cmp_key(root, node, index, key) <= 0)
                break;
        }

        if (index == layout->keynum)
            return NULL;

        node = bnode_get_value(root, node, index);
        if (!node)
            return NULL;
    }

    return node;
}

export void *
bfdev_btree_lookup(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node;
    unsigned int index;

    node = bnode_lookup(root, key);
    if (!node)
        return NULL;

    index = bnode_key_index(root, node, key);
    if (index == layout->keynum)
        return NULL;

    return bnode_get_value(root, node, index);
}

export int
btree_update(struct bfdev_btree_root *root, uintptr_t *key, void *value)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node;
    unsigned int index;

    node = bnode_lookup(root, key);
    if (!node)
        return -BFDEV_ENOENT;

    index = bnode_key_index(root, node, key);
    if (index == layout->keynum)
        return -BFDEV_ENOENT;

    bnode_set_value(root, node, index, value);
    return 0;
}

static int
btree_extend(struct bfdev_btree_root *root)
{
    struct bfdev_btree_node *node;
    unsigned int index;

    node = bnode_alloc(root);
    if (unlikely(!node))
        return -BFDEV_ENOMEM;

    if (likely(root->node)) {
        index = bnode_fill_index(root, root->node, 0);
        bnode_set_key(root, node, 0, bnode_get_key(root, root->node, index - 1));
        bnode_set_value(root, node, 0, root->node);
    }

    root->node = node;
    root->height++;

    return 0;
}

static void
btree_tailor(struct bfdev_btree_root *root)
{
    struct bfdev_btree_node *node = root->node;

    if (root->height < 2)
        return;

    root->height--;
    root->node = bnode_get_value(root, node, 0);
    bnode_free(root, node);
}

static int
insert_level(struct bfdev_btree_root *root, unsigned int level,
             uintptr_t *key, void *value)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node, *new;
    unsigned int index, fill, count;
    uintptr_t *halfkey;
    int retval;

    if (root->height < level) {
        retval = btree_extend(root);
        if (retval)
            return retval;
    }

    for (;;) {
        node = bnode_find_parent(root, key, level);
        index = bnode_find_index(root, node, key);
        fill = bnode_fill_index(root, node, index);

        if (index < fill && !bnode_cmp_key(root, node, index, key)) {
            if (root->clash) {
                halfkey = bnode_get_value(root, node, index);
                return root->clash(root, halfkey, value);
            }
            return -BFDEV_EALREADY;
        }

        if (fill != layout->keynum)
            break;

        new = bnode_alloc(root);
        if (unlikely(!new))
            return -BFDEV_ENOMEM;

        halfkey = bnode_get_key(root, node, fill / 2 - 1);
        retval = insert_level(root, level + 1, halfkey, new);

        if (unlikely(retval)) {
            bnode_free(root, new);
            return retval;
        }

        /* split node entry */
        for (count = 0; count < fill / 2; ++count) {
            bnode_set_key(root, new, count, bnode_get_key(root, node, count));
            bnode_set_value(root, new, count, bnode_get_value(root, node, count));
            bnode_set_key(root, node, count, bnode_get_key(root, node, count + fill / 2));
            bnode_set_value(root, node, count, bnode_get_value(root, node, count + fill / 2));
            bnode_clear_index(root, node, count + fill / 2);
        }

        if (fill & 1) {
            bnode_set_key(root, new, count, bnode_get_key(root, node, fill - 1));
            bnode_set_value(root, new, count, bnode_get_value(root, node, fill - 1));
            bnode_clear_index(root, node, fill - 1);
        }
    }

    /* shift nodes and insert */
    for (count = fill; count > index; --count) {
        bnode_set_key(root, node, count, bnode_get_key(root, node, count - 1));
        bnode_set_value(root, node, count, bnode_get_value(root, node, count - 1));
    }

    bnode_set_key(root, node, index, key);
    bnode_set_value(root, node, index, value);

    return 0;
}

export int
bfdev_btree_insert(struct bfdev_btree_root *root, uintptr_t *key, void *value)
{
    if (!value)
        return -BFDEV_EINVAL;
    return insert_level(root, 1, key, value);
}

static void *
remove_level(struct bfdev_btree_root *root, unsigned int level, uintptr_t *key);

static void
rebalance_merge(struct bfdev_btree_root *root, unsigned int level,
                struct bfdev_btree_node *lnode, unsigned int lfill,
                struct bfdev_btree_node *rnode, unsigned int rfill,
                struct bfdev_btree_node *parent, unsigned int index)

{
    unsigned int count;

    for (count = 0; count < rfill; ++count) {
        bnode_set_key(root, lnode, lfill + count, bnode_get_key(root, rnode, count));
        bnode_set_value(root, lnode, lfill + count, bnode_get_value(root, rnode, count));
    }

    bnode_set_value(root, parent, index, rnode);
    bnode_set_value(root, parent, index + 1, lnode);
    remove_level(root, level + 1, bnode_get_key(root, parent, index));
    bnode_free(root, rnode);
}

static void
remove_rebalance(struct bfdev_btree_root *root, unsigned int level,
                 uintptr_t *key, struct bfdev_btree_node *child,
                 unsigned int fill)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *parent, *node;
    unsigned int index, nfill;

    if (!fill) {
        remove_level(root, level + 1, key);
        bnode_free(root, child);
        return;
    }

    parent = bnode_find_parent(root, key, level + 1);
    index = bnode_find_index(root, parent, key);

    if (index) {
        node = bnode_get_value(root, parent, index - 1);
        nfill = bnode_fill_index(root, node, 0);
		if (fill + nfill <= layout->keynum) {
            rebalance_merge(
                root, level, node, nfill,
                child, fill, parent, index - 1
            );
            return;
        }
    }

    if (index + 1 < bnode_fill_index(root, parent, index)) {
        node = bnode_get_value(root, parent, index + 1);
        nfill = bnode_fill_index(root, node, 0);
		if (fill + nfill <= layout->keynum) {
            rebalance_merge(
                root, level, child, fill,
                node, nfill, parent, index
            );
            return;
        }
    }
}

static void *
remove_level(struct bfdev_btree_root *root, unsigned int level,
             uintptr_t *key)
{
    struct bfdev_btree_layout *layout = root->layout;
    struct bfdev_btree_node *node;
    unsigned int index, last, count;
    void *clash, *value = NULL;

    if (level > root->height) {
        root->height = 0;
        root->node = NULL;
        return NULL;
    }

    node = bnode_find_parent(root, key, level);
    index = bnode_find_index(root, node, key);
    last = bnode_fill_index(root, node, index) - 1;

    if (level == 1) {
        if (bnode_cmp_key(root, node, index, key))
            return NULL;

        value = bnode_get_value(root, node, index);
        if (root->remove) {
            clash = root->remove(root, value);
            if (clash)
                return clash;
        }
    }

    /* shift nodes and remove */
    for (count = index; count < last; ++count) {
        bnode_set_key(root, node, count, bnode_get_key(root, node, count + 1));
        bnode_set_value(root, node, count, bnode_get_value(root, node, count + 1));
    }
    bnode_clear_index(root, node, count);

    /* rebalance node tree */
    if (last < layout->keynum / 2) {
        if (level < root->height)
            remove_rebalance(root, level, key, node, last);
        else if (last == 1)
            btree_tailor(root);
    }

    return value;
}

export void *
bfdev_btree_remove(struct bfdev_btree_root *root, uintptr_t *key)
{
    if (!root->height)
        return NULL;
    return remove_level(root, 1, key);
}

export void
bfdev_btree_destroy(struct bfdev_btree_root *root)
{
    struct bfdev_btree_layout *layout = root->layout;
    uintptr_t key[layout->keylen], tkey[layout->keylen];
    void *value, *tval;

    bfdev_btree_for_each_safe(root, key, value, tkey, tval)
        bfdev_btree_remove(root, key);

    bnode_free(root, root->node);
    root->node = NULL;
    root->height = 0;
}

export void
bfdev_btree_key_copy(struct bfdev_btree_root *root,
                    uintptr_t *dest, uintptr_t *src)
{
    struct bfdev_btree_layout *layout = root->layout;
    memcpy(dest, src, layout->keylen * sizeof(uintptr_t));
}

export void *
bfdev_btree_first(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_node *node = root->node;
    unsigned int height = root->height;

    if (!height)
        return NULL;

    while (--height)
        node = bnode_get_value(root, node, 0);

    bnode_takeout_key(root, node, 0, key);
    return bnode_get_value(root, node, 0);
}

export void *
bfdev_btree_last(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_node *node = root->node;
    unsigned int last, height = root->height;

    if (!height)
        return NULL;

    while (--height) {
        last = bnode_fill_index(root, node, 0) - 1;
        node = bnode_get_value(root, node, last);
    }

    last = bnode_fill_index(root, node, 0) - 1;
    bnode_takeout_key(root, node, last, key);
    return bnode_get_value(root, node, last);
}

export void *
bfdev_btree_next(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_node *node;
    unsigned int depth, index, fill;

    if (btree_empty_key(root, key))
        return NULL;

    for (depth = 1; depth <= root->height; ++depth) {
        node = bnode_find_parent(root, key, depth);
        index = bnode_find_index(root, node, key);
        fill = bnode_fill_index(root, node, index);
        if (++index < fill)
            break;
    }

    if (depth > root->height)
        return NULL;

    while (--depth) {
        node = bnode_get_value(root, node, index);
        index = 0;
    }

    bnode_takeout_key(root, node, index, key);
    return bnode_get_value(root, node, index);
}

export void *
bfdev_btree_prev(struct bfdev_btree_root *root, uintptr_t *key)
{
    struct bfdev_btree_node *node;
    unsigned int depth, index;

    if (btree_empty_key(root, key))
        return NULL;

    for (depth = 1; depth <= root->height; ++depth) {
        node = bnode_find_parent(root, key, depth);
        index = bnode_find_index(root, node, key);
        if (index--)
            break;
    }

    if (depth > root->height)
        return NULL;

    while (--depth) {
        node = bnode_get_value(root, node, index);
        index = bnode_fill_index(root, node, 0) - 1;
    }

    bnode_takeout_key(root, node, index, key);
    return bnode_get_value(root, node, index);
}
