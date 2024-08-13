/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/heap.h>
#include <bfdev/bitops.h>
#include <bfdev/titer.h>
#include <export.h>

static __bfdev_always_inline void
parent_swap(bfdev_heap_root_t *root, bfdev_heap_node_t *parent,
            bfdev_heap_node_t *node)
{
    bfdev_heap_node_t shadow, *gparent;

    if (node->left)
        node->left->parent = parent;
    if (node->right)
        node->right->parent = parent;

    shadow = *node;
    if (parent->left == node) {
        node->left = parent;
        node->right = parent->right;
        if (node->right)
            parent->right->parent = node;
    } else { /* parent->right == node */
        node->right = parent;
        node->left = parent->left;
        if (node->left)
            parent->left->parent = node;
    }

    gparent = parent->parent;
    node->parent = gparent;

    if (!gparent)
        root->node = node;
    else if (gparent->left == parent)
        gparent->left = node;
    else /* gparent->right == parent */
        gparent->right = node;

    *parent = shadow;
    parent->parent = node;
}

export void
bfdev_heap_fixup(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                 bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_node_t *parent;

    while ((parent = node->parent)) {
        if (cmp(node, parent, pdata) >= 0)
            break;
        parent_swap(root, parent, node);
    }
}

export void
bfdev_heap_erase(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                 bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_node_t *child1, *child2, *successor;

    successor = node->parent;
    if (successor && cmp(node, successor, pdata) < 0) {
        bfdev_heap_fixup(root, node, cmp, pdata);
        return;
    }

    for (;;) {
        child1 = node->left;
        child2 = node->right;

        if (!child1 && !child2)
            return;
        else if (!child2)
            successor = node->left;
        else if (!child1)
            successor = node->left;
        else { /* child1 && child2 */
            if (cmp(node->left, node->right, pdata) < 0)
                successor = node->left;
            else
                successor = node->right;
        }

        if (cmp(node, successor, pdata) < 0)
            return;
        parent_swap(root, node, successor);
    }
}

export bfdev_heap_node_t *
bfdev_heap_remove(bfdev_heap_root_t *root, bfdev_heap_node_t *node)
{
    bfdev_heap_node_t *successor;

    successor = bfdev_heap_find(root, root->count);
    if (!successor->parent) {
        /*
         * Case 1: node to delete is root.
         *
         *   (r)      (p)
         *    |        |
         *   (n)  ->  null
         *
         */

        root->node = NULL;
        successor = NULL;
    } else {
        bfdev_heap_node_t *parent;

        parent = successor->parent;
        if (parent->left == successor)
            parent->left = NULL;
        else /* parent->right == successor */
            parent->right = NULL;

        /*
         * Case 2: node to delete is successor.
         *
         *     (p)          (p)
         *     / \          /
         *   (s) (n)  ->  (s)
         *
         */

        if (node == successor) {
            successor = NULL;
            goto finish;
        }

        /*
         * Case 3: node to delete is normal.
         *
         *         (p)              (p)
         *        /   \             / \
         *       /     \          (x) (s)
         *     (n)     (s)  ->    / \
         *     / \     /        (z) (y)
         *   (x) (y) (z)
         *
         */

        /* inheritance information */
        *successor = *node;

        /* inheritance relationship */
        if (node->left)
            node->left->parent = successor;
        if (node->right)
            node->right->parent = successor;

        if (!(parent = node->parent))
            root->node = successor;
        else {
            if (node == parent->left)
                parent->left = successor;
            else if (node == parent->right)
                parent->right = successor;
        }
    }

finish:
    root->count--;
    return successor;
}

export bfdev_heap_node_t **
bfdev_heap_parent(bfdev_heap_root_t *root, bfdev_heap_node_t **parentp,
                  bfdev_heap_node_t *node)
{
    bfdev_heap_node_t **link;
    unsigned int depth;

    link = &root->node;
    if (bfdev_unlikely(!*link)) {
        *parentp = NULL;
        return link;
    }

    depth = bfdev_flsuf(root->count + 1);
    while (depth--) {
        *parentp = *link;
        if ((root->count + 1) & BFDEV_BIT(depth))
            link = &(*link)->right;
        else
            link = &(*link)->left;
    };

    return link;
}

export bfdev_heap_node_t *
bfdev_heap_find(bfdev_heap_root_t *root, unsigned long index)
{
    bfdev_heap_node_t *node;
    unsigned int depth;

    depth = bfdev_flsuf(root->count);
    node = root->node;

    while (node && depth--) {
        if (index & BFDEV_BIT(depth))
            node = node->right;
        else
            node = node->left;
    };

    return node;
}

BFDEV_TITER_BASE_DEFINE(
    export, bfdev_heap,
    bfdev_heap_node_t, left, right
)

BFDEV_TITER_LEVELORDER_DEFINE(
    export, bfdev_heap, bfdev_heap,
    bfdev_heap_root_t, node,
    bfdev_heap_node_t, left, right
)

BFDEV_TITER_POSTORDER_DEFINE(
    export, bfdev_heap_post, bfdev_heap,
    bfdev_heap_root_t, node,
    bfdev_heap_node_t, parent, left, right
)
