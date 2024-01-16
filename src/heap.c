/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/heap.h>
#include <bfdev/bitops.h>
#include <bfdev/titer.h>
#include <export.h>

export void
bfdev_heap_fixup(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                 bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_node_t *parent;

    while ((parent = node->parent)) {
        if (cmp(node, parent, pdata) >= 0)
            break;
        bfdev_bintree_inherit(&root->btree, node);
    }
}

export void
bfdev_heap_erase(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                 bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_node_t *successor = node->parent;
    bfdev_heap_node_t *child1, *child2;

    if (successor && cmp(node, successor, pdata) < 0)
        bfdev_heap_fixup(root, node, cmp, pdata);

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
            if (cmp(node->left, node->right, pdata) < 0)
                successor = node->left;
            else
                successor = node->right;
        }

        if (cmp(node, successor, pdata) < 0)
            return;

        bfdev_bintree_inherit(&root->btree, successor);
    }
}

export bfdev_heap_node_t *
bfdev_heap_remove(bfdev_heap_root_t *root, bfdev_heap_node_t *node)
{
    bfdev_heap_node_t *successor = bfdev_heap_find(root, root->count);

    if (!successor->parent) {
        /*
         * Case 1: node to delete is root.
         *
         *   (r)      (p)
         *    |        |
         *   (n)  ->  null
         *
         */

        root->btree.node = NULL;
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
            root->btree.node = successor;
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
    unsigned int depth = bfdev_flsuf(root->count + 1);
    bfdev_heap_node_t **link;

    link = &root->btree.node;
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

export bfdev_heap_node_t *
bfdev_heap_find(bfdev_heap_root_t *root, unsigned int index)
{
    unsigned int depth = bfdev_flsuf(root->count);
    bfdev_heap_node_t *node = root->btree.node;

    while (node && depth--) {
        if ((root->count) & BFDEV_BIT(depth))
            node = node->right;
        else
            node = node->left;
    };

    return node;
}
