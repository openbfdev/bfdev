/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/heap.h>
#include <bfdev/bitops.h>
#include <bfdev/titer.h>
#include <export.h>

static __bfdev_always_inline void
parent_swap(struct bfdev_heap_root *root, struct bfdev_heap_node *parent,
            struct bfdev_heap_node *node)
{
    struct bfdev_heap_node *gparent = parent->parent;
    struct bfdev_heap_node shadow = *node;

    if (node->left)
        node->left->parent = parent;
    if (node->right)
        node->right->parent = parent;

    if (parent->left == node) {
        node->left = parent;
        if ((node->right = parent->right))
            parent->right->parent = node;
    } else { /* parent->right == node */
        node->right = parent;
        if ((node->left = parent->left))
            parent->left->parent = node;
    }

    if (!(node->parent = gparent))
        root->node = node;
    else if (gparent->left == parent)
        gparent->left = node;
    else /* gparent->right == parent */
        gparent->right = node;

    *parent = shadow;
    parent->parent = node;
}

/**
 * bfdev_heap_fixup - balance after insert node.
 * @root: heap root of node.
 * @node: new inserted node.
 * @cmp: operator defining the node order.
 */
export void
bfdev_heap_fixup(struct bfdev_heap_root *root, struct bfdev_heap_node *node,
                 bfdev_heap_cmp_t cmp)
{
    struct bfdev_heap_node *parent;

    while ((parent = node->parent)) {
        if (cmp(node, parent) >= 0)
            break;
        parent_swap(root, parent, node);
    }
}

/**
 * bfdev_heap_erase - balance after remove node.
 * @root: heap root of node.
 * @node: removed node.
 * @cmp: operator defining the node order.
 */
export void
bfdev_heap_erase(struct bfdev_heap_root *root, struct bfdev_heap_node *node,
                 bfdev_heap_cmp_t cmp)
{
    struct bfdev_heap_node *successor = node->parent;
    struct bfdev_heap_node *child1, *child2;

    if (successor && cmp(node, successor) < 0)
        bfdev_heap_fixup(root, node, cmp);

    else for (;;) {
        child1 = node->left;
        child2 = node->right;

        if (!child1 && !child2)
            return;
        else if (!child2)
            successor = node->left;
        else if (!child1)
            successor = node->left;
        else { /* child1 && child2 */
            if (cmp(node->left, node->right) < 0)
                successor = node->left;
            else
                successor = node->right;
        }

        if (cmp(node, successor) < 0)
            return;
        parent_swap(root, node, successor);
    }
}

/**
 * bfdev_heap_remove - remove node form heap.
 * @root: heap root of node.
 * @node: node to remove.
 */
export struct bfdev_heap_node *
bfdev_heap_remove(struct bfdev_heap_root *root, struct bfdev_heap_node *node)
{
    struct bfdev_heap_node *successor = bfdev_heap_find(root, root->count);

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
        struct bfdev_heap_node *parent;

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

/**
 * bfdev_heap_parent - find the parent node.
 * @root: heap tree want to search.
 * @parentp: pointer used to modify the parent node pointer.
 * @node: new node to insert.
 */
export struct bfdev_heap_node **
bfdev_heap_parent(struct bfdev_heap_root *root, struct bfdev_heap_node **parentp,
                  struct bfdev_heap_node *node)
{
    unsigned int depth = bfdev_flsuf(root->count + 1);
    struct bfdev_heap_node **link;

    link = &root->node;
    if (bfdev_unlikely(!*link)) {
        *parentp = NULL;
        return link;
    }

    while (depth--) {
        *parentp = *link;
        if ((root->count + 1) & BFDEV_BIT(depth))
            link = &(*link)->right;
        else
            link = &(*link)->left;
    };

    return link;
}

/**
 * bfdev_heap_find - find @index in tree @root.
 * @root: heap tree want to search.
 * @index: index of node.
 */
export struct bfdev_heap_node *
bfdev_heap_find(struct bfdev_heap_root *root, unsigned int index)
{
    unsigned int depth = bfdev_flsuf(root->count);
    struct bfdev_heap_node *node = root->node;

    while (node && depth--) {
        if ((root->count) & BFDEV_BIT(depth))
            node = node->right;
        else
            node = node->left;
    };

    return node;
}

BFDEV_TITER_BASE_DEFINE(
    export, bfdev_heap,
    struct bfdev_heap_node, left, right
)

BFDEV_TITER_LEVELORDER_DEFINE(
    export, bfdev_heap, bfdev_heap,
    struct bfdev_heap_root, node,
    struct bfdev_heap_node, left, right
)
