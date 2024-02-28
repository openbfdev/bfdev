/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/rbtree.h>
#include <bfdev/callback.h>
#include <bfdev/titer.h>
#include <export.h>

/**
 * child_change - replace old child by new one.
 * @root: rbtree root of node.
 * @parent: parent to change child.
 * @oldn: node to be replaced.
 * @newn: new node to insert.
 */
static __bfdev_always_inline void
child_change(bfdev_rb_root_t *root, bfdev_rb_node_t *parent,
             bfdev_rb_node_t *oldn, bfdev_rb_node_t *newn)
{
    if (!parent)
        root->node = newn;
    else if (parent->left == oldn)
        parent->left = newn;
    else
        parent->right = newn;
}

/**
 * rotate_set - replace old child by new one.
 * @root: rbtree root of node.
 * @node: parent to change child.
 * @newn: node to be replaced.
 * @child: managed child node.
 * @color: color after rotation.
 * @ccolor: color of child.
 * @callbacks: augmented callback function.
 */
static __bfdev_always_inline void
rotate_set(bfdev_rb_root_t *root, bfdev_rb_node_t *node, bfdev_rb_node_t *newn,
           bfdev_rb_node_t *child, unsigned int color, unsigned int ccolor,
           const bfdev_rb_callbacks_t *callbacks)
{
    bfdev_rb_node_t *parent;

    parent = node->parent;
    newn->parent = node->parent;
    node->parent = newn;

    if (color != BFDEV_RB_NSET) {
        newn->color = node->color;
        node->color = color;
    }

    if (child) {
        if (ccolor != BFDEV_RB_NSET)
            child->color = ccolor;
        child->parent = node;
    }

    child_change(root, parent, node, newn);
    callbacks->rotate(node, newn);
}

/**
 * left_rotate - left rotation one node.
 * @root: rbtree root of node.
 * @node: node to rotation.
 * @color: color after rotation.
 * @ccolor: color of child.
 * @callbacks: augmented callback function.
 */
static __bfdev_always_inline bfdev_rb_node_t *
left_rotate(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
            unsigned int color, unsigned int ccolor,
            const bfdev_rb_callbacks_t *callbacks)
{
    bfdev_rb_node_t *child, *successor;

    /* change left child */
    successor = node->right;
    child = successor->left;

    node->right = child;
    successor->left = node;
    rotate_set(root, node, successor, child, color, ccolor, callbacks);

    return child;
}

/**
 * right_rotate - right rotation one node.
 * @root: rbtree root of node.
 * @node: node to rotation.
 * @color: color after rotation.
 * @ccolor: color of child.
 * @callbacks: augmented callback function.
 */
static __bfdev_always_inline bfdev_rb_node_t *
right_rotate(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
             unsigned int color, unsigned int ccolor,
             const bfdev_rb_callbacks_t *callbacks)
{
    bfdev_rb_node_t *child, *successor;

    /* change right child */
    successor = node->left;
    child = successor->right;

    node->left = child;
    successor->right = node;
    rotate_set(root, node, successor, child, color, ccolor, callbacks);

    return child;
}

export void
bfdev_rb_fixup_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                         const bfdev_rb_callbacks_t *callbacks)
{
    bfdev_rb_node_t *parent, *gparent, *tmp;

    while (root && node) {
        parent = node->parent;

        /*
         * The inserted node is root. Either this is the
         * first node, or we recursed at Case 1 below and
         * are no longer violating 4).
         */

        if (bfdev_unlikely(!parent)) {
            node->color = BFDEV_RB_BLACK;
            break;
        }

        /*
         * If there is a black parent, we are done.
         * Otherwise, take some corrective action as,
         * per 4), we don't want a red root or two
         * consecutive red nodes.
         */

        if (parent->color == BFDEV_RB_BLACK)
            break;

        gparent = parent->parent;
        tmp = gparent->right;

        if (tmp != parent) {
            /*
             * Case 1 - node's uncle is red (color flips).
             *
             *       G            g
             *      / \          / \
             *     p   t  -->   P   T
             *    /            /
             *   n            n
             *
             * However, since g's parent might be red, and
             * 4) does not allow this, we need to recurse
             * at g.
             */

            if (tmp && tmp->color == BFDEV_RB_RED) {
                parent->color = tmp->color = BFDEV_RB_BLACK;
                gparent->color = BFDEV_RB_RED;
                node = gparent;
                continue;
            }

            /*
             * Case 2 - node's uncle is black and node is
             * the parent's right child (left rotate at parent).
             *
             *      G             G
             *     / \           / \
             *    p   U  -->    n   U
             *     \           /
             *      n         p
             *     /           \
             *    c             C
             *
             * This still leaves us in violation of 4), the
             * continuation into Case 3 will fix that.
             */

            if (node == parent->right)
                left_rotate(root, parent, BFDEV_RB_NSET, BFDEV_RB_BLACK, callbacks);

            /*
             * Case 3 - node's uncle is black and node is
             * the parent's left child (right rotate at gparent).
             *
             *        G           P
             *       / \         / \
             *      p   U  -->  n   g
             *     / \             / \
             *    n   s           S   U
             */

            right_rotate(root, gparent, BFDEV_RB_RED, BFDEV_RB_BLACK, callbacks);
        } else {
            /* parent == gparent->right */
            tmp = gparent->left;

            /* Case 1 - color flips */
            if (tmp && tmp->color == BFDEV_RB_RED) {
                parent->color = tmp->color = BFDEV_RB_BLACK;
                gparent->color = BFDEV_RB_RED;
                node = gparent;
                continue;
            }

            /* Case 2 - right rotate at parent */
            if (node == parent->left)
                right_rotate(root, parent, BFDEV_RB_NSET, BFDEV_RB_BLACK, callbacks);

            /* Case 3 - left rotate at gparent */
            left_rotate(root, gparent, BFDEV_RB_RED, BFDEV_RB_BLACK, callbacks);
        }

        break;
    }
}

export void
bfdev_rb_erase_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *parent,
                         const bfdev_rb_callbacks_t *callbacks)
{
    bfdev_rb_node_t *sibling, *node = NULL;
    bfdev_rb_node_t *tmp1, *tmp2;

    while (root && parent) {
        /*
         * Loop invariants:
         * - node is black (or NULL on first iteration)
         * - node is not the root (parent is not NULL)
         * - All leaf paths going through parent and node have a
         *   black node count that is 1 lower than other leaf paths.
         */

        sibling = parent->right;
        if (node != sibling) {
            /*
             * Case 1 - left rotate at parent
             *
             *     P               S
             *    / \             / \
             *   N   s    -->    p   Sr
             *      / \         / \
             *     Sl  Sr      N   Sl
             */

            if (sibling->color == BFDEV_RB_RED)
                sibling = left_rotate(root, parent, BFDEV_RB_RED, BFDEV_RB_BLACK, callbacks);

            tmp2 = sibling->right;
            if (!tmp2 || tmp2->color == BFDEV_RB_BLACK) {
                tmp1 = sibling->left;

                /*
                 * Case 2 - sibling color flip
                 * (p could be either color here)
                 *
                 *    (p)           (p)
                 *    / \           / \
                 *   N   S    -->  N   s
                 *      / \           / \
                 *     Sl  Sr        Sl  Sr
                 *
                 * This leaves us violating 5) which
                 * can be fixed by flipping p to black
                 * if it was red, or by recursing at p.
                 * p is red when coming from Case 1.
                 */

                if (!tmp1 || tmp1->color == BFDEV_RB_BLACK) {
                    sibling->color = BFDEV_RB_RED;
                    if (parent->color == BFDEV_RB_RED)
                        parent->color = BFDEV_RB_BLACK;
                    else {
                        node = parent;
                        parent = node->parent;
                        if (parent)
                            continue;
                    }
                    break;
                }

                /*
                 * Case 3 - right rotate at sibling
                 * (p could be either color here)
                 *
                 *   (p)           (p)
                 *   / \           / \
                 *  N   S    -->  N   sl
                 *     / \             \
                 *    sl  Sr            S
                 *      \              / \
                 *       t            T   Sr
                 *
                 * Note: p might be red, and then both
                 * p and sl are red after rotation(which
                 * breaks property 4). This is fixed in
                 * Case 4 (in __rb_rotate_set_parents()
                 *         which set sl the color of p
                 *         and set p black)
                 *
                 *   (p)            (sl)
                 *   / \            /  \
                 *  N   sl   -->   P    S
                 *       \        /      \
                 *        S      N        Sr
                 *         \
                 *          Sr
                 */

                right_rotate(root, sibling, BFDEV_RB_NSET, BFDEV_RB_BLACK, callbacks);
                tmp2 = sibling;
            }

            /*
             * Case 4 - left rotate at parent + color flips
             * (p and sl could be either color here.
             *  After rotation, p becomes black, s acquires
             *  p's color, and sl keeps its color)
             *
             *      (p)             (s)
             *      / \             / \
             *     N   S     -->   P   Sr
             *        / \         / \
             *      (sl) sr      N  (sl)
             */

            left_rotate(root, parent, BFDEV_RB_BLACK, BFDEV_RB_NSET, callbacks);
            tmp2->color = BFDEV_RB_BLACK;
        } else {
            sibling = parent->left;

            /* Case 1 - right rotate at parent */
            if (sibling->color == BFDEV_RB_RED)
                sibling = right_rotate(root, parent, BFDEV_RB_RED, BFDEV_RB_BLACK, callbacks);

            tmp1 = sibling->left;
            if (!tmp1 || tmp1->color == BFDEV_RB_BLACK) {
                tmp2 = sibling->right;

                /* Case 2 - sibling color flip */
                if (!tmp2 || tmp2->color == BFDEV_RB_BLACK) {
                    sibling->color = BFDEV_RB_RED;
                    if (parent->color == BFDEV_RB_RED)
                        parent->color = BFDEV_RB_BLACK;
                    else {
                        node = parent;
                        parent = node->parent;
                        if (parent)
                            continue;
                    }
                    break;
                }

                /* Case 3 - left rotate at sibling */
                left_rotate(root, sibling, BFDEV_RB_NSET, BFDEV_RB_BLACK, callbacks);
                tmp1 = sibling;
            }

            /* Case 4 - right rotate at parent + color flips */
            right_rotate(root, parent, BFDEV_RB_BLACK, BFDEV_RB_NSET, callbacks);
            tmp1->color = BFDEV_RB_BLACK;
        }

        break;
    }
}

export bfdev_rb_node_t *
bfdev_rb_remove_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                          const bfdev_rb_callbacks_t *callbacks)
{
    bfdev_rb_node_t *parent, *rebalance = NULL;
    bfdev_rb_node_t *child1, *child2;

    parent = node->parent;
    child1 = node->left;
    child2 = node->right;

    if (!child1 && !child2) {
        /*
         * Case 1: node to erase has no child.
         *
         *     (p)        (p)
         *     / \          \
         *   (n) (s)  ->    (s)
         *
         */

        if (node->color == BFDEV_RB_BLACK)
            rebalance = parent;
        child_change(root, parent, node, NULL);
    } else if (!child2) {
        /*
         * Case 1: node to erase only has left child.
         *
         *      (p)          (p)
         *      / \          / \
         *    (n) (s)  ->  (c) (s)
         *    /
         *  (c)
         *
         */

        child1->color = node->color;
        child1->parent = node->parent;
        child_change(root, parent, node, child1);
    } else if (!child1) {
        /*
         * Case 1: node to erase only has right child.
         *
         *    (p)          (p)
         *    / \          / \
         *  (n) (s)  ->  (c) (s)
         *    \
         *    (c)
         */

        child2->color = node->color;
        child2->parent = node->parent;
        child_change(root, parent, node, child2);
    } else { /* child1 && child2 */
        bfdev_rb_node_t *tmp, *successor = child2;

        child1 = child2->left;
        if (!child1) {
            /*
             * Case 2: node's successor is its right child
             *
             *    (n)          (s)
             *    / \          / \
             *  (x) (s)  ->  (x) (c)
             *        \
             *        (c)
             */

            parent = successor;
            tmp = successor->right;
            callbacks->copy(node, successor);
        } else {
            /*
             * Case 3: node's successor is leftmost under
             * node's right child subtree
             *
             *    (n)          (s)
             *    / \          / \
             *  (x) (y)  ->  (x) (y)
             *      /            /
             *    (p)          (p)
             *    /            /
             *  (s)          (c)
             *    \
             *    (c)
             */

            do {
                parent = successor;
                successor = child1;
                child1 = child1->left;
            } while (child1);

            tmp = successor->right;
            parent->left = tmp;
            successor->right = child2;
            child2->parent = successor;

            callbacks->copy(node, successor);
            callbacks->propagate(parent, successor);
        }

        child1 = node->left;
        successor->left = child1;
        child1->parent = successor;

        child1 = node->parent;
        child_change(root, child1, node, successor);

        if (tmp) {
            tmp->parent = parent;
            tmp->color = BFDEV_RB_BLACK;
        } else if (successor->color == BFDEV_RB_BLACK)
            rebalance = parent;

        successor->parent = child1;
        successor->color = node->color;
        parent = successor;
    }

    callbacks->propagate(parent, NULL);
    return rebalance;
}

static const bfdev_rb_callbacks_t
dummy_callbacks = {
    .rotate = bfdev_dummy_noop,
    .copy = bfdev_dummy_noop,
    .propagate = bfdev_dummy_noop,
};

export void
bfdev_rb_fixup(bfdev_rb_root_t *root, bfdev_rb_node_t *node)
{
    bfdev_rb_fixup_augmented(root, node, &dummy_callbacks);
}

export void
bfdev_rb_erase(bfdev_rb_root_t *root, bfdev_rb_node_t *parent)
{
    bfdev_rb_erase_augmented(root, parent, &dummy_callbacks);
}

export bfdev_rb_node_t *
bfdev_rb_remove(bfdev_rb_root_t *root, bfdev_rb_node_t *node)
{
    return bfdev_rb_remove_augmented(root, node, &dummy_callbacks);
}

export void
bfdev_rb_replace(bfdev_rb_root_t *root, bfdev_rb_node_t *oldn,
                 bfdev_rb_node_t *newn)
{
    bfdev_rb_node_t *parent;

    parent = oldn->parent;
    *newn = *oldn;

    if (oldn->left)
        oldn->left->parent = newn;
    if (oldn->right)
        oldn->right->parent = newn;

    child_change(root, parent, oldn, newn);
}

export bfdev_rb_node_t *
bfdev_rb_find(const bfdev_rb_root_t *root, void *key,
              bfdev_rb_find_t cmp)
{
    bfdev_rb_node_t *node;
    long retval;

    node = root->node;
    while (node) {
        retval = cmp(node, key);
        if (retval == LONG_MIN)
            return NULL;

        if (retval > 0)
            node = node->left;
        else if (retval < 0)
            node = node->right;
        else
            return node;
    }

    return NULL;
}

export bfdev_rb_node_t *
bfdev_rb_find_last(bfdev_rb_root_t *root, void *key, bfdev_rb_find_t cmp,
                   bfdev_rb_node_t **parentp, bfdev_rb_node_t ***linkp)
{
    long retval;

    *linkp = &root->node;
    if (bfdev_unlikely(!**linkp)) {
        *parentp = NULL;
        return NULL;
    }

    do {
        retval = cmp((*parentp = **linkp), key);
        if (retval == LONG_MIN)
            return NULL;

        if (retval > 0)
            *linkp = &(**linkp)->left;
        else if (retval < 0)
            *linkp = &(**linkp)->right;
        else
            return **linkp;
    } while (**linkp);

    return NULL;
}

export bfdev_rb_node_t **
bfdev_rb_parent(bfdev_rb_root_t *root, bfdev_rb_node_t **parentp,
                bfdev_rb_node_t *node, bfdev_rb_cmp_t cmp, void *pdata,
                bool *leftmostp)
{
    bfdev_rb_node_t **link;
    bool leftmost;
    long retval;

    link = &root->node;
    leftmost = true;

    if (bfdev_unlikely(!*link)) {
        *parentp = NULL;
        goto finish;
    }

    do {
        retval = cmp(node, (*parentp = *link), pdata);
        if (bfdev_unlikely(!retval))
            return NULL;

        if (retval < 0)
            link = &(*link)->left;
        else {
            link = &(*link)->right;
            leftmost = false;
        }
    } while (*link);

finish:
    if (leftmostp)
        *leftmostp = leftmost;

    return link;
}

BFDEV_TITER_BASE_DEFINE(
    export, bfdev_rb,
    bfdev_rb_node_t, left, right
)

BFDEV_TITER_INORDER_DEFINE(
    export, bfdev_rb, bfdev_rb,
    bfdev_rb_root_t, node,
    bfdev_rb_node_t, parent, left, right
)

BFDEV_TITER_PREORDER_DEFINE(
    export, bfdev_rb_pre, bfdev_rb,
    bfdev_rb_root_t, node,
    bfdev_rb_node_t, parent, left, right
)

BFDEV_TITER_POSTORDER_DEFINE(
    export, bfdev_rb_post, bfdev_rb,
    bfdev_rb_root_t, node,
    bfdev_rb_node_t, parent, left, right
)
