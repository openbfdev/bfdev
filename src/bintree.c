/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/bintree.h>
#include <export.h>

/**
 * child_change - replace old child by new one.
 * @root: bintreetree root of node.
 * @parent: parent to change child.
 * @oldn: node to be replaced.
 * @newn: new node to insert.
 */
static __bfdev_always_inline void
change_child(bfdev_bintree_root_t *root, bfdev_bintree_node_t *parent,
             bfdev_bintree_node_t *oldn, bfdev_bintree_node_t *newn)
{
    if (!parent)
        root->node = newn;
    else if (parent->left == oldn)
        parent->left = newn;
    else /* parent->right == node */
        parent->right = newn;
}

export bool
bfdev_bintree_inherit(bfdev_bintree_root_t *root, bfdev_bintree_node_t *node)
{
    bfdev_bintree_node_t *parent, *gparent;
    bfdev_bintree_node_t shadow;

    parent = node->parent;
    if (bfdev_unlikely(!parent))
        return false;

    shadow = *node;
    if (node->left)
        node->left->parent = parent;
    if (node->right)
        node->right->parent = parent;

    if (parent->left == node) {
        node->left = parent;
        if (parent->right)
            parent->right->parent = node;
        node->right = parent->right;
    } else { /* parent->right == node */
        node->right = parent;
        if (parent->left)
            parent->left->parent = node;
        node->left = parent->left;
    }

    gparent = parent->parent;
    node->parent = gparent;
    change_child(root, gparent, parent, node);

    *parent = shadow;
    parent->parent = node;

    return true;
}

export bfdev_bintree_node_t *
bfdev_bintree_left_far(const bfdev_bintree_node_t *node)
{
    /* Go left as we can */
    while (node->left)
        node = node->left;

    return (bfdev_bintree_node_t *)node;
}

export bfdev_bintree_node_t *
bfdev_bintree_right_far(const bfdev_bintree_node_t *node)
{
    /* Go right as we can */
    while (node->right)
        node = node->right;

    return (bfdev_bintree_node_t *)node;
}

export bfdev_bintree_node_t *
bfdev_bintree_left_deep(const bfdev_bintree_node_t *node)
{
    /* Go left deep as we can */
    while (node) {
        if (node->left)
            node = node->left;
        else if (node->right)
            node = node->right;
        else
            return (bfdev_bintree_node_t *)node;
    }

    return NULL;
}

export bfdev_bintree_node_t *
bfdev_bintree_right_deep(const bfdev_bintree_node_t *node)
{
    /* Go right deep as we can */
    while (node) {
        if (node->right)
            node = node->right;
        else if (node->left)
            node = node->left;
        else
            return (bfdev_bintree_node_t *)node;
    }

    return NULL;
}

export bfdev_bintree_node_t *
bfdev_bintree_first(const bfdev_bintree_root_t *root)
{
    bfdev_bintree_node_t *node;

    node = root->node;
    if (!node)
        return NULL;

    /* Get the leftmost node */
    node = bfdev_bintree_left_far(node);
    return node;
}

export bfdev_bintree_node_t *
bfdev_bintree_last(const bfdev_bintree_root_t *root)
{
    bfdev_bintree_node_t *node;

    node = root->node;
    if (!node)
        return NULL;

    /* Get the rightmost node */
    node = bfdev_bintree_right_far(node);
    return node;
}

export bfdev_bintree_node_t *
bfdev_bintree_prev(const bfdev_bintree_node_t *node)
{
    bfdev_bintree_node_t *parent;

    if (!node)
        return NULL;

    /*
     * If there is a left-hand node, go down
     * and then as far right as possible.
     */
    if (node->left) {
        node = node->left;
        return bfdev_bintree_right_far(node);
    }

    /*
     * No left-hand children. Go up till we find an ancestor
     * which is a right-hand child of its parent.
     */
    while ((parent = node->parent) && node != parent->right)
        node = parent;

    return parent;
}

export bfdev_bintree_node_t *
bfdev_bintree_next(const bfdev_bintree_node_t *node)
{
    bfdev_bintree_node_t *parent;

    if (!node)
        return NULL;

    /*
     * If there is a right-hand node, go down
     * and then as far left as possible.
     */
    if (node->right) {
        node = node->right;
        return bfdev_bintree_left_far(node);
    }

    /*
     * No right-hand children. Go up till we find an ancestor
     * which is a left-hand child of its parent.
     */
    while ((parent = node->parent) && node != parent->left)
        node = parent;

    return parent;
}

export bfdev_bintree_node_t *
bfdev_bintree_pre_first(const bfdev_bintree_root_t *root)
{
    /* Get the root node */
    return root->node;
}

export bfdev_bintree_node_t *
bfdev_bintree_pre_next(const bfdev_bintree_node_t *node)
{
    bfdev_bintree_node_t *parent;

    if (!node)
        return NULL;

    /**
     * If there are left and right child nodes,
     * then we iterate directly.
     */
    if (node->left)
        return node->left;

    if (node->right)
        return node->right;

    /**
     * If we have no children, Go up till we find an ancestor
     * which have a another right-hand child.
     */
    while ((parent = node->parent) &&
           (!parent->right || node == parent->right))
        node = parent;

    return parent ? parent->right : NULL;
}

export bfdev_bintree_node_t *
bfdev_bintree_post_first(const bfdev_bintree_root_t *root)
{
    bfdev_bintree_node_t *node;

    node = root->node;
    if (!node)
        return NULL;

    /* Get the left deepest node. */
    return bfdev_bintree_left_deep(node);
}

export bfdev_bintree_node_t *
bfdev_bintree_post_next(const bfdev_bintree_node_t *node)
{
    const bfdev_bintree_node_t *parent;

    if (!node)
        return NULL;
    parent = node->parent;

    /**
     * If we are the parent's left node, go to the parent's right
     * node then all the way down to the left.
     * Otherwise we are the parent's right node, and the parent
     * should be next.
     */
    if (parent && node == parent->left && parent->right)
        return bfdev_bintree_left_deep(parent->right);
    else
        return (bfdev_bintree_node_t *)parent;
}

export bfdev_bintree_node_t *
bfdev_bintree_level_first(const bfdev_bintree_root_t *root, unsigned long *index)
{
    /* Initialize counter and get root node */
    *index = 0;
    return root->node;
}

export bfdev_bintree_node_t *
bfdev_bintree_level_next(const bfdev_bintree_root_t *root, unsigned long *index)
{
    unsigned int depth = bfdev_flsuf(++*index + 1);
    bfdev_bintree_node_t *node;

    node = root->node;
    while (node && depth--) {
        if ((*index + 1) & BFDEV_BIT(depth))
            node = node->right;
        else
            node = node->left;
    };

    return node;
}
