/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HEAP_H_
#define _BFDEV_HEAP_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

struct bfdev_heap_node {
    struct bfdev_heap_node *parent;
    struct bfdev_heap_node *left;
    struct bfdev_heap_node *right;
};

struct bfdev_heap_root {
    struct bfdev_heap_node *node;
    unsigned int count;
};

#define BFDEV_HEAP_STATIC \
    {NULL, 0}

#define BFDEV_HEAP_CACHED_STATIC \
    {{NULL}, NULL}

#define BFDEV_HEAP_INIT \
    (struct bfdev_heap_root) BFDEV_HEAP_STATIC

#define BFDEV_HEAP_ROOT(name) \
    struct bfdev_heap_root name = BFDEV_HEAP_INIT

#define BFDEV_HEAP_EMPTY_ROOT(root) \
    ((root)->node == NULL)

#define BFDEV_HEAP_EMPTY_NODE(node) \
    ((node)->parent == (node))

#define BFDEV_HEAP_CLEAR_NODE(node) \
    ((node)->parent = (node))

#define BFDEV_HEAP_ROOT_NODE(root) \
    ((root)->node)

#define BFDEV_HEAP_NODE_COUNT(root) \
    ((root)->count)

/**
 * bfdev_heap_entry - get the struct for this entry.
 * @ptr: the &struct bfdev_heap_node pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define	bfdev_heap_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * bfdev_heap_entry_safe - get the struct for this entry or null.
 * @ptr: the &struct bfdev_heap_node pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_entry_safe(ptr, type, member) \
    container_of_safe(ptr, type, member)

#ifdef BFDEV_DEBUG_HEAP
extern bool bfdev_heap_debug_link_check(struct bfdev_heap_node *parent, struct bfdev_heap_node **link, struct bfdev_heap_node *node);
extern bool bfdev_heap_debug_delete_check(struct bfdev_heap_node *node);
#endif

typedef long (*bfdev_heap_cmp_t)(const struct bfdev_heap_node *nodea, const struct bfdev_heap_node *nodeb);
extern void bfdev_heap_fixup(struct bfdev_heap_root *root, struct bfdev_heap_node *node, bfdev_heap_cmp_t cmp);
extern void bfdev_heap_erase(struct bfdev_heap_root *root, struct bfdev_heap_node *node, bfdev_heap_cmp_t cmp);
extern struct bfdev_heap_node *bfdev_heap_remove(struct bfdev_heap_root *root, struct bfdev_heap_node *node);
extern struct bfdev_heap_node **bfdev_heap_parent(struct bfdev_heap_root *root, struct bfdev_heap_node **parentp, struct bfdev_heap_node *node);
extern struct bfdev_heap_node *bfdev_heap_find(struct bfdev_heap_root *root, unsigned int index);

extern struct bfdev_heap_node *bfdev_heap_first(const struct bfdev_heap_root *root, unsigned long *index);
extern struct bfdev_heap_node *bfdev_heap_next(const struct bfdev_heap_root *root, unsigned long *index);

/**
 * bfdev_heap_first_entry - get the preorder first element from a heaptree.
 * @ptr: the heaptree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_first_entry(root, index, type, member) \
    bfdev_heap_entry_safe(bfdev_heap_first(root, index), type, member)

/**
 * bfdev_heap_next_entry - get the preorder next element in heaptree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_next_entry(root, index, type, member) \
    bfdev_heap_entry_safe(bfdev_heap_next(root, index), type, member)

/**
 * bfdev_heap_for_each - preorder iterate over a heaptree.
 * @pos: the &struct bfdev_heap_node to use as a loop cursor.
 * @root: the root for your heaptree.
 */
#define bfdev_heap_for_each(pos, index, root) \
    for (pos = bfdev_heap_first(root, index); \
         pos; pos = bfdev_heap_next(root, index))

/**
 * bfdev_heap_for_each_from - preorder iterate over a heaptree from the current point.
 * @pos: the &struct bfdev_heap_node to use as a loop cursor.
 */
#define bfdev_heap_for_each_from(pos, index, root) \
    for (; pos; pos = bfdev_heap_next(root, index))

/**
 * bfdev_heap_for_each_continue - continue preorder iteration over a heaptree.
 * @pos: the &struct bfdev_heap_node to use as a loop cursor.
 */
#define bfdev_heap_for_each_continue(pos, index, root) \
    for (pos = bfdev_heap_next(root, index); \
         pos; pos = bfdev_heap_next(root, index))

/**
 * bfdev_heap_for_each_entry - preorder iterate over heaptree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your heaptree.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry(pos, index, root, member) \
    for (pos = bfdev_heap_first_entry(root, index, typeof(*pos), member); \
         pos; pos = bfdev_heap_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_heap_for_each_entry_from - preorder iterate over heaptree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry_from(pos, index, root, member) \
    for (; pos; pos = bfdev_heap_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_heap_for_each_entry_continue - continue preorder iteration over heaptree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry_continue(pos, index, root, member) \
    for (pos = bfdev_heap_next_entry(root, index, typeof(*pos), member); \
         pos; pos = bfdev_heap_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_heap_link - link node to parent.
 * @root: heaptree root of node.
 * @parent: point to parent node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void bfdev_heap_link(struct bfdev_heap_root *root, struct bfdev_heap_node *parent,
                             struct bfdev_heap_node **link, struct bfdev_heap_node *node)
{
#ifdef BFDEV_DEBUG_HEAP
    if (bfdev_unlikely(!bfdev_heap_debug_link_check(parent, link, node)))
        return;
#endif

    /* link = &parent->left/right */
    *link = node;
    node->parent = parent;
    node->left = node->right = NULL;
    root->count++;
}

/**
 * bfdev_heap_insert_node - link node to parent and fixup heaptree.
 * @root: heaptree root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void
bfdev_heap_insert_node(struct bfdev_heap_root *root, struct bfdev_heap_node *parent,
                       struct bfdev_heap_node **link, struct bfdev_heap_node *node,
                       bfdev_heap_cmp_t cmp)
{
    bfdev_heap_link(root, parent, link, node);
    bfdev_heap_fixup(root, node, cmp);
}

/**
 * bfdev_heap_insert - find the parent node and insert new node.
 * @root: heaptree root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline void
bfdev_heap_insert(struct bfdev_heap_root *root, struct bfdev_heap_node *node,
                  bfdev_heap_cmp_t cmp)
{
    struct bfdev_heap_node *parent, **link;

    link = bfdev_heap_parent(root, &parent, node);
    bfdev_heap_insert_node(root, parent, link, node, cmp);
}

/**
 * bfdev_heap_delete - delete node and fixup heaptree.
 * @root: heaptree root of node.
 * @node: node to delete.
 */
static inline void
bfdev_heap_delete(struct bfdev_heap_root *root, struct bfdev_heap_node *node,
                  bfdev_heap_cmp_t cmp)
{
    struct bfdev_heap_node *rebalance;

#ifdef BFDEV_DEBUG_HEAP
    if (bfdev_unlikely(!bfdev_heap_debug_delete_check(node)))
        return;
#endif

    if ((rebalance = bfdev_heap_remove(root, node)))
        bfdev_heap_erase(root, rebalance, cmp);

    node->left = BFDEV_POISON_HPNODE1;
    node->right = BFDEV_POISON_HPNODE2;
    node->parent = BFDEV_POISON_HPNODE3;
}

BFDEV_END_DECLS

#endif /* _BFDEV_HEAP_H_ */
