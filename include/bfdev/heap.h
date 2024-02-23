/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HEAP_H_
#define _BFDEV_HEAP_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_heap_root bfdev_heap_root_t;
typedef struct bfdev_heap_node bfdev_heap_node_t;

struct bfdev_heap_node {
    bfdev_heap_node_t *parent;
    bfdev_heap_node_t *left;
    bfdev_heap_node_t *right;
};

struct bfdev_heap_root {
    bfdev_heap_node_t *node;
    unsigned long count;
};

#define BFDEV_HEAP_STATIC \
    {NULL, 0}

#define BFDEV_HEAP_INIT \
    (bfdev_heap_root_t) BFDEV_HEAP_STATIC

#define BFDEV_HEAP_ROOT(name) \
    bfdev_heap_root_t name = BFDEV_HEAP_INIT

#define BFDEV_HEAP_ROOT_NODE(root) \
    ((root)->node)

#define BFDEV_HEAP_ROOT_COUNT(root) \
    ((root)->count)

/**
 * bfdev_heap_entry - get the struct for this entry.
 * @ptr: the &bfdev_heap_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_heap_entry_safe - get the struct for this entry or null.
 * @ptr: the &bfdev_heap_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_entry_safe(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

#ifdef BFDEV_DEBUG_HEAP
extern bool
bfdev_heap_check_link(bfdev_heap_node_t *parent, bfdev_heap_node_t **link,
                      bfdev_heap_node_t *node);

extern bool
bfdev_heap_check_delete(bfdev_heap_node_t *node);
#endif

BFDEV_CALLBACK_CMP(
    bfdev_heap_cmp_t,
    const bfdev_heap_node_t *
);

static inline void
bfdev_heap_init(bfdev_heap_root_t *root)
{
    *root = BFDEV_HEAP_INIT;
}

static inline bool
bfdev_heap_empty_root(bfdev_heap_root_t *root)
{
    return !root->node;
}

/**
 * bfdev_heap_fixup - balance after insert node.
 * @root: heap root of node.
 * @node: new inserted node.
 * @cmp: operator defining the node order.
 */
extern void
bfdev_heap_fixup(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                 bfdev_heap_cmp_t cmp, void *pdata);

/**
 * bfdev_heap_erase - balance after remove node.
 * @root: heap root of node.
 * @node: removed node.
 * @cmp: operator defining the node order.
 */
extern void
bfdev_heap_erase(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                 bfdev_heap_cmp_t cmp, void *pdata);

/**
 * bfdev_heap_remove - remove node form heap.
 * @root: heap root of node.
 * @node: node to remove.
 */
extern bfdev_heap_node_t *
bfdev_heap_remove(bfdev_heap_root_t *root, bfdev_heap_node_t *node);

/**
 * bfdev_heap_parent - find the parent node.
 * @root: heap tree want to search.
 * @parentp: pointer used to modify the parent node pointer.
 * @node: new node to insert.
 */
extern bfdev_heap_node_t **
bfdev_heap_parent(bfdev_heap_root_t *root, bfdev_heap_node_t **parentp,
                  bfdev_heap_node_t *node);

/**
 * bfdev_heap_find - find @index in tree @root.
 * @root: heap tree want to search.
 * @index: index of node.
 */
extern bfdev_heap_node_t *
bfdev_heap_find(bfdev_heap_root_t *root, unsigned long index);

/**
 * bfdev_heap_link - link node to parent.
 * @root: heap root of node.
 * @parent: point to parent node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void
bfdev_heap_link(bfdev_heap_root_t *root, bfdev_heap_node_t *parent,
                bfdev_heap_node_t **link, bfdev_heap_node_t *node)
{
#ifdef BFDEV_DEBUG_HEAP
    if (bfdev_unlikely(!bfdev_heap_check_link(parent, link, node)))
        return;
#endif

    /* link = &parent->left/right */
    *link = node;
    node->parent = parent;
    node->left = node->right = NULL;
    root->count++;
}

/**
 * bfdev_heap_insert_node - link node to parent and fixup heap.
 * @root: heap root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void
bfdev_heap_insert_node(bfdev_heap_root_t *root, bfdev_heap_node_t *parent,
                       bfdev_heap_node_t **link, bfdev_heap_node_t *node,
                       bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_link(root, parent, link, node);
    bfdev_heap_fixup(root, node, cmp, pdata);
}

/**
 * bfdev_heap_insert - find the parent node and insert new node.
 * @root: heap root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline void
bfdev_heap_insert(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                  bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_node_t *parent, **link;

    link = bfdev_heap_parent(root, &parent, node);
    bfdev_heap_insert_node(root, parent, link, node, cmp, pdata);
}

/**
 * bfdev_heap_delete - delete node and fixup heap.
 * @root: heap root of node.
 * @node: node to delete.
 */
static inline void
bfdev_heap_delete(bfdev_heap_root_t *root, bfdev_heap_node_t *node,
                  bfdev_heap_cmp_t cmp, void *pdata)
{
    bfdev_heap_node_t *rebalance;

#ifdef BFDEV_DEBUG_HEAP
    if (bfdev_unlikely(!bfdev_heap_check_delete(node)))
        return;
#endif

    if ((rebalance = bfdev_heap_remove(root, node)))
        bfdev_heap_erase(root, rebalance, cmp, pdata);

    node->left = BFDEV_POISON_HPNODE1;
    node->right = BFDEV_POISON_HPNODE2;
    node->parent = BFDEV_POISON_HPNODE3;
}

/* Base iteration - basic iteration helper */
extern bfdev_heap_node_t *
bfdev_heap_left_far(const bfdev_heap_node_t *node);

extern bfdev_heap_node_t *
bfdev_heap_right_far(const bfdev_heap_node_t *node);

extern bfdev_heap_node_t *
bfdev_heap_left_deep(const bfdev_heap_node_t *node);

extern bfdev_heap_node_t *
bfdev_heap_right_deep(const bfdev_heap_node_t *node);

/* Level iteration (Sequential) - access in level sequence */
extern bfdev_heap_node_t *
bfdev_heap_first(const bfdev_heap_root_t *root, unsigned long *index);

extern bfdev_heap_node_t *
bfdev_heap_next(const bfdev_heap_root_t *root, unsigned long *index);

/* Postorder iteration (Depth-first) - always visit the parent after its children */
extern bfdev_heap_node_t *
bfdev_heap_post_first(const bfdev_heap_root_t *root);

extern bfdev_heap_node_t *
bfdev_heap_post_next(const bfdev_heap_node_t *node);

/**
 * bfdev_heap_first_entry - get the preorder first element from a heap.
 * @root: the heap root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_first_entry(root, index, type, member) \
    bfdev_heap_entry_safe(bfdev_heap_first(root, index), type, member)

/**
 * bfdev_heap_next_entry - get the preorder next element in heap.
 * @root: the heap root to take the element from.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_next_entry(root, index, type, member) \
    bfdev_heap_entry_safe(bfdev_heap_next(root, index), type, member)

/**
 * bfdev_heap_for_each - preorder iterate over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @root: the heap root to take the element from.
 */
#define bfdev_heap_for_each(pos, index, root) \
    for (pos = bfdev_heap_first(root, index); \
         pos; pos = bfdev_heap_next(root, index))

/**
 * bfdev_heap_for_each_from - preorder iterate over a heap from the current point.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @root: the heap root to take the element from.
 */
#define bfdev_heap_for_each_from(pos, index, root) \
    for (; pos; pos = bfdev_heap_next(root, index))

/**
 * bfdev_heap_for_each_continue - continue preorder iteration over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @root: the heap root to take the element from.
 */
#define bfdev_heap_for_each_continue(pos, index, root) \
    for (pos = bfdev_heap_next(root, index); \
         pos; pos = bfdev_heap_next(root, index))

/**
 * bfdev_heap_for_each_entry - preorder iterate over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the heap root to take the element from.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry(pos, index, root, member) \
    for (pos = bfdev_heap_first_entry(root, index, typeof(*pos), member); \
         pos; pos = bfdev_heap_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_heap_for_each_entry_from - preorder iterate over heap of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @root: the heap root to take the element from.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry_from(pos, index, root, member) \
    for (; pos; pos = bfdev_heap_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_heap_for_each_entry_continue - continue preorder iteration over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the heap root to take the element from.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry_continue(pos, index, root, member) \
    for (pos = bfdev_heap_next_entry(root, index, typeof(*pos), member); \
         pos; pos = bfdev_heap_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_heap_post_first_entry - get the postorder first element from a heap.
 * @ptr: the heap root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_first_entry(ptr, type, member) \
    bfdev_heap_entry_safe(bfdev_heap_post_first(ptr), type, member)

/**
 * bfdev_heap_post_next_entry - get the postorder next element in heap.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_next_entry(pos, member) \
    bfdev_heap_entry_safe(bfdev_heap_post_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_heap_post_for_each - postorder iterate over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @root: the root for your heap.
 */
#define bfdev_heap_post_for_each(pos, root) \
    for (pos = bfdev_heap_post_first(root); pos; pos = bfdev_heap_post_next(pos))

/**
 * bfdev_heap_post_for_each_from - postorder iterate over a heap from the current point.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 */
#define bfdev_heap_post_for_each_from(pos) \
    for (; pos; pos = bfdev_heap_post_next(pos))

/**
 * bfdev_heap_post_for_each_continue - continue postorder iteration over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 */
#define bfdev_heap_post_for_each_continue(pos) \
    for (pos = bfdev_heap_post_next(pos); pos; pos = bfdev_heap_post_next(pos))

/**
 * bfdev_heap_post_for_each_safe - postorder iterate over a heap safe against removal of heap entry.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @tmp: another bfdev_heap_node to use as temporary storage.
 * @root: the root for your heap.
 */
#define bfdev_heap_post_for_each_safe(pos, tmp, root) \
    for (pos = bfdev_heap_post_first(root); pos && \
        ({tmp = bfdev_heap_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_heap_post_for_each_safe_from - postorder iterate over a heap safe against removal of heap entry from the current point.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @tmp: another bfdev_heap_node to use as temporary storage.
 */
#define bfdev_heap_post_for_each_safe_from(pos, tmp) \
    for (; pos && ({tmp = bfdev_heap_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_heap_post_for_each_safe_continue - continue heap postorder iteration safe against removal.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @tmp: another bfdev_heap_node to use as temporary storage.
 */
#define bfdev_heap_post_for_each_safe_continue(pos, tmp) \
    for (pos = bfdev_heap_post_next(pos); pos && \
        ({tmp = bfdev_heap_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_heap_post_for_each_entry - postorder iterate over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your heap.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry(pos, root, member) \
    for (pos = bfdev_heap_post_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_heap_post_next_entry(pos, member))

/**
 * bfdev_heap_post_for_each_entry_from - postorder iterate over heap of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_heap_post_next_entry(pos, member))

/**
 * bfdev_heap_post_for_each_entry_continue - continue postorder iteration over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_continue(pos, member) \
    for (pos = bfdev_heap_post_next_entry(pos, member); \
         pos; pos = bfdev_heap_post_next_entry(pos, member))

/**
 * bfdev_heap_post_for_each_entry_safe - postorder iterate over heap of given type safe against removal of heap entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @root: the root for your heap.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_safe(pos, tmp, root, member) \
    for (pos = bfdev_heap_post_first_entry(root, typeof(*pos), member); \
         pos && ({ tmp = bfdev_heap_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_heap_post_for_each_entry_from_safe - postorder iterate over heap from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_from_safe(pos, tmp, member) \
    for (; pos && ({ tmp = bfdev_heap_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_heap_post_for_each_entry_continue_safe - continue postorder heap iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_continue_safe(pos, tmp, member) \
    for (pos = bfdev_heap_post_next_entry(pos, member); \
         pos && ({ tmp = bfdev_heap_post_next_entry(pos, member); \
         1; }); pos = tmp)


BFDEV_END_DECLS

#endif /* _BFDEV_HEAP_H_ */
