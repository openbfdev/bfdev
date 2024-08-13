/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/btree.h>
#include <export.h>

static __bfdev_always_inline void *
bnode_get_value(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                unsigned int index)
{
    bfdev_btree_layout_t *layout;
    unsigned int offset;

    layout = root->layout;
    offset = layout->ptrindex + index;

    return (void *)node->block[offset];
}

static __bfdev_always_inline void
bnode_set_value(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                unsigned int index, void *value)
{
    bfdev_btree_layout_t *layout;
    unsigned int offset;

    layout = root->layout;
    offset = layout->ptrindex + index;

    node->block[offset] = (uintptr_t)value;
}

static __bfdev_always_inline uintptr_t *
bnode_get_key(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
              unsigned int index)
{
    bfdev_btree_layout_t *layout;
    unsigned int offset;

    layout = root->layout;
    offset = layout->keylen * index;

    return &node->block[offset];
}

static __bfdev_always_inline void
bnode_set_key(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
              unsigned int index, uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    uintptr_t *slot;
    size_t size;

    layout = root->layout;
    size = layout->keylen * sizeof(uintptr_t);

    slot = bnode_get_key(root, node, index);
    bfport_memcpy(slot, key, size);
}

static inline long
bnode_cmp_key(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
              unsigned int index, uintptr_t *key)
{
    const bfdev_btree_ops_t *ops;
    uintptr_t *slot;

    ops = root->ops;
    slot = bnode_get_key(root, node, index);

    return ops->find(root, slot, key);
}

static inline void
bnode_takeout_key(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                  unsigned int index, uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    uintptr_t *slot;
    size_t size;

    layout = root->layout;
    size = layout->keylen * sizeof(uintptr_t);

    slot = bnode_get_key(root, node, index);
    bfport_memcpy(key, slot, size);
}

static inline void
bnode_clear_index(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                  unsigned int index)
{
    bfdev_btree_layout_t *layout;
    uintptr_t *slot;
    size_t size;

    layout = root->layout;
    size = layout->keylen * sizeof(uintptr_t);

    bnode_set_value(root, node, index, NULL);
    slot = bnode_get_key(root, node, index);
    bfport_memset(slot, 0, size);
}

static inline bool
btree_empty_key(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    unsigned int count;

    layout = root->layout;
    for (count = 0; count < layout->keylen; ++count) {
        if (key[count])
            return false;
    }

    return true;
}

static inline void
bnode_migrate(bfdev_btree_root_t *root,
              bfdev_btree_node_t *nnode, unsigned nindex,
              bfdev_btree_node_t *onode, unsigned oindex)
{
    void *slot;

    slot = bnode_get_key(root, onode, oindex);
    bnode_set_key(root, nnode, nindex, slot);

    slot = bnode_get_value(root, onode, oindex);
    bnode_set_value(root, nnode, nindex, slot);
}

static inline bfdev_btree_node_t *
bnode_alloc(bfdev_btree_root_t *root)
{
    const bfdev_btree_ops_t *ops;
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node;

    layout = root->layout;
    ops = root->ops;

    node = ops->alloc(root);
    if (bfdev_likely(node))
        bfport_memset(node, 0, layout->nodesize);

    return node;
}

static inline void
bnode_free(bfdev_btree_root_t *root, bfdev_btree_node_t *node)
{
    const bfdev_btree_ops_t *ops;

    ops = root->ops;

    return ops->free(root, node);
}

static unsigned int
bnode_fill_index(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                 unsigned int start)
{
    bfdev_btree_layout_t *layout;
    unsigned int index;

    layout = root->layout;
    for (index = start; index < layout->keynum; ++index) {
        if (!bnode_get_value(root, node, index))
            break;
    }

    return index;
}

static unsigned int
bnode_key_index(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    unsigned int index;
    long retval;

    layout = root->layout;
    for (index = 0; index < layout->keynum; ++index) {
        retval = bnode_cmp_key(root, node, index, key);
        if (retval < 0)
            break;
        else if (retval == 0)
            return index;
    }

    return layout->keynum;
}

static bfdev_btree_node_t *
bnode_find_parent(bfdev_btree_root_t *root, uintptr_t *key, unsigned int level)
{
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node;
    unsigned int height, index;

    layout = root->layout;
    node = root->node;

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

static unsigned int
bnode_find_index(bfdev_btree_root_t *root, bfdev_btree_node_t *node,
                 uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    unsigned int index;

    layout = root->layout;
    for (index = 0; index < layout->keynum; ++index) {
        if (bnode_cmp_key(root, node, index, key) <= 0)
            break;
    }

    return index;
}

static bfdev_btree_node_t *
bnode_lookup(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node;
    unsigned int height, index;

    height = root->height;
    if (!height)
        return NULL;

    layout = root->layout;
    node = root->node;

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
bfdev_btree_lookup(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node;
    unsigned int index;

    node = bnode_lookup(root, key);
    if (!node)
        return NULL;

    layout = root->layout;
    index = bnode_key_index(root, node, key);

    if (index == layout->keynum)
        return NULL;

    return bnode_get_value(root, node, index);
}

export int
bfdev_btree_update(bfdev_btree_root_t *root, uintptr_t *key, void *value)
{
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node;
    unsigned int index;

    node = bnode_lookup(root, key);
    if (!node)
        return -BFDEV_ENOENT;

    layout = root->layout;
    index = bnode_key_index(root, node, key);

    if (index == layout->keynum)
        return -BFDEV_ENOENT;

    bnode_set_value(root, node, index, value);
    return -BFDEV_ENOERR;
}

static int
btree_extend(bfdev_btree_root_t *root)
{
    bfdev_btree_node_t *node;
    unsigned int index;
    uintptr_t *slot;

    node = bnode_alloc(root);
    if (bfdev_unlikely(!node))
        return -BFDEV_ENOMEM;

    if (bfdev_likely(root->node)) {
        index = bnode_fill_index(root, root->node, 0);
        slot = bnode_get_key(root, root->node, index - 1);
        bnode_set_key(root, node, 0, slot);
        bnode_set_value(root, node, 0, root->node);
    }

    root->node = node;
    root->height++;

    return -BFDEV_ENOERR;
}

static void
btree_shrink(bfdev_btree_root_t *root)
{
    bfdev_btree_node_t *node;

    if (root->height < 2)
        return;

    node = root->node;
    root->height--;

    root->node = bnode_get_value(root, node, 0);
    bnode_free(root, node);
}

static int
insert_level(bfdev_btree_root_t *root, unsigned int level,
             uintptr_t *key, void *value)
{
    const bfdev_btree_ops_t *ops;
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node, *newn;
    unsigned int index, fill, count;
    uintptr_t *halfkey;
    int retval;

    if (root->height < level) {
        retval = btree_extend(root);
        if (retval)
            return retval;
    }

    layout = root->layout;
    ops = root->ops;

    for (;;) {
        node = bnode_find_parent(root, key, level);
        index = bnode_find_index(root, node, key);
        fill = bnode_fill_index(root, node, index);

        if (index < fill && !bnode_cmp_key(root, node, index, key)) {
            if (ops->clash) {
                halfkey = bnode_get_value(root, node, index);
                return ops->clash(root, halfkey, value);
            }
            return -BFDEV_EALREADY;
        }

        if (fill != layout->keynum)
            break;

        newn = bnode_alloc(root);
        if (bfdev_unlikely(!newn))
            return -BFDEV_ENOMEM;

        halfkey = bnode_get_key(root, node, fill / 2 - 1);
        retval = insert_level(root, level + 1, halfkey, newn);

        if (bfdev_unlikely(retval)) {
            bnode_free(root, newn);
            return retval;
        }

        /* split node entry */
        for (count = 0; count < fill / 2; ++count) {
            bnode_migrate(root, newn, count, node, count);
            bnode_migrate(root, node, count, node, count + fill / 2);
            bnode_clear_index(root, node, count + fill / 2);
        }

        if (fill & 1) {
            bnode_migrate(root, newn, count, node, fill - 1);
            bnode_clear_index(root, node, fill - 1);
        }
    }

    /* shift nodes and insert */
    for (count = fill; count > index; --count)
        bnode_migrate(root, node, count, node, count - 1);

    bnode_set_key(root, node, index, key);
    bnode_set_value(root, node, index, value);

    return -BFDEV_ENOERR;
}

static void *
remove_level(bfdev_btree_root_t *root, unsigned int level, uintptr_t *key);

static void
rebalance_merge(bfdev_btree_root_t *root, unsigned int level,
                bfdev_btree_node_t *lnode, unsigned int lfill,
                bfdev_btree_node_t *rnode, unsigned int rfill,
                bfdev_btree_node_t *parent, unsigned int index)
{
    unsigned int count;

    for (count = 0; count < rfill; ++count)
        bnode_migrate(root, lnode, lfill + count, rnode, count);

    bnode_set_value(root, parent, index, rnode);
    bnode_set_value(root, parent, index + 1, lnode);

    remove_level(root, level + 1, bnode_get_key(root, parent, index));
    bnode_free(root, rnode);
}

static void
remove_rebalance(bfdev_btree_root_t *root, unsigned int level, uintptr_t *key,
                 bfdev_btree_node_t *child, unsigned int fill)
{
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *parent, *node;
    unsigned int index, nfill;

    if (!fill) {
        remove_level(root, level + 1, key);
        bnode_free(root, child);
        return;
    }

    layout = root->layout;
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
remove_level(bfdev_btree_root_t *root, unsigned int level, uintptr_t *key)
{
    const bfdev_btree_ops_t *ops;
    bfdev_btree_layout_t *layout;
    bfdev_btree_node_t *node;
    unsigned int index, last, count;
    void *clash, *value;

    if (level > root->height) {
        root->height = 0;
        root->node = NULL;
        return NULL;
    }

    layout = root->layout;
    ops = root->ops;

    node = bnode_find_parent(root, key, level);
    index = bnode_find_index(root, node, key);
    last = bnode_fill_index(root, node, index) - 1;

    value = NULL;
    if (level == 1) {
        if (bnode_cmp_key(root, node, index, key))
            return NULL;

        value = bnode_get_value(root, node, index);
        if (ops->remove) {
            clash = ops->remove(root, value);
            if (clash)
                return clash;
        }
    }

    /* shift nodes and remove */
    for (count = index; count < last; ++count)
        bnode_migrate(root, node, count, node, count + 1);
    bnode_clear_index(root, node, count);

    /* rebalance node tree */
    if (last < layout->keynum / 2) {
        if (level < root->height)
            remove_rebalance(root, level, key, node, last);
        else if (last == 1)
            btree_shrink(root);
    }

    return value;
}

export int
bfdev_btree_insert(bfdev_btree_root_t *root, uintptr_t *key, void *value)
{
    if (bfdev_unlikely(!value))
        return -BFDEV_EINVAL;

    return insert_level(root, 1, key, value);
}

export void *
bfdev_btree_remove(bfdev_btree_root_t *root, uintptr_t *key)
{
    if (bfdev_unlikely(!root->height))
        return NULL;

    return remove_level(root, 1, key);
}

export void
bfdev_btree_release(bfdev_btree_root_t *root, bfdev_release_t release,
                    void *pdata)
{
    bfdev_btree_layout_t *layout;
    uintptr_t *key, *tkey;
    void *value, *tval;

    layout = root->layout;
    key = bfdev_alloca(sizeof(uintptr_t) * layout->keylen);
    tkey = bfdev_alloca(sizeof(uintptr_t) * layout->keylen);

    bfdev_btree_for_each_safe(root, key, value, tkey, tval) {
        if (release)
            release(value, pdata);
        bfdev_btree_remove(root, key);
    }

    bnode_free(root, root->node);
    root->node = NULL;
    root->height = 0;
}

export void
bfdev_btree_key_copy(bfdev_btree_root_t *root, uintptr_t *dest, uintptr_t *src)
{
    bfdev_btree_layout_t *layout;

    layout = root->layout;
    bfport_memcpy(dest, src, layout->keylen * sizeof(uintptr_t));
}

export void *
bfdev_btree_first(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_node_t *node;
    unsigned int height;

    height = root->height;
    if (!height)
        return NULL;

    node = root->node;
    while (--height)
        node = bnode_get_value(root, node, 0);

    bnode_takeout_key(root, node, 0, key);
    return bnode_get_value(root, node, 0);
}

export void *
bfdev_btree_last(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_node_t *node;
    unsigned int last, height;

    height = root->height;
    if (!height)
        return NULL;

    node = root->node;
    while (--height) {
        last = bnode_fill_index(root, node, 0) - 1;
        node = bnode_get_value(root, node, last);
    }

    last = bnode_fill_index(root, node, 0) - 1;
    bnode_takeout_key(root, node, last, key);
    return bnode_get_value(root, node, last);
}

export void *
bfdev_btree_next(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_node_t *node;
    unsigned int depth, index, fill;
    unsigned int height;

    height = root->height;
    if (bfdev_unlikely(!height))
        return NULL;

    if (btree_empty_key(root, key))
        return NULL;

    for (depth = 1; depth <= height; ++depth) {
        node = bnode_find_parent(root, key, depth);
        index = bnode_find_index(root, node, key);
        fill = bnode_fill_index(root, node, index);
        if (++index < fill)
            break;
    }

    if (depth > height)
        return NULL;

    while (--depth) {
        node = bnode_get_value(root, node, index);
        index = 0;
    }

    bnode_takeout_key(root, node, index, key);
    return bnode_get_value(root, node, index);
}

export void *
bfdev_btree_prev(bfdev_btree_root_t *root, uintptr_t *key)
{
    bfdev_btree_node_t *node;
    unsigned int depth, index;
    unsigned int height;

    height = root->height;
    if (bfdev_unlikely(!height))
        return NULL;

    if (btree_empty_key(root, key))
        return NULL;

    for (depth = 1; depth <= height; ++depth) {
        node = bnode_find_parent(root, key, depth);
        index = bnode_find_index(root, node, key);
        if (index--)
            break;
    }

    if (depth > height)
        return NULL;

    while (--depth) {
        node = bnode_get_value(root, node, index);
        index = bnode_fill_index(root, node, 0) - 1;
    }

    bnode_takeout_key(root, node, index, key);
    return bnode_get_value(root, node, index);
}
