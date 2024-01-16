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
#include <bfdev/bintree.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_heap_root bfdev_heap_root_t;
typedef bfdev_bintree_node_t bfdev_heap_node_t;

struct bfdev_heap_root {
    bfdev_bintree_root_t btree;
    unsigned int count;
};

#define BFDEV_HEAP_STATIC \
    {BFDEV_BINTREE_STATIC, 0}

#define BFDEV_HEAP_CACHED_STATIC \
    {{NULL}, NULL}

#define BFDEV_HEAP_INIT \
    (bfdev_heap_root_t) BFDEV_HEAP_STATIC

#define BFDEV_HEAP_ROOT(name) \
    bfdev_heap_root_t name = BFDEV_HEAP_INIT

#define BFDEV_HEAP_ROOT_NODE(root) \
    BFDEV_BINTREE_ROOT_NODE(&(root)->btree)

#define BFDEV_HEAP_NODE_COUNT(root) \
    ((root)->count)

/**
 * bfdev_heap_entry - get the struct for this entry.
 * @ptr: the &bfdev_heap_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define	bfdev_heap_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_heap_entry_safe - get the struct for this entry or null.
 * @ptr: the &bfdev_heap_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_entry_safe(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

BFDEV_CALLBACK_CMP(
    bfdev_heap_cmp_t,
    const bfdev_heap_node_t *
);

static __bfdev_always_inline void
bfdev_heap_init(bfdev_heap_root_t *root)
{
    *root = BFDEV_HEAP_INIT;
}

static __bfdev_always_inline bool
bfdev_heap_empty_root(bfdev_heap_root_t *root)
{
    return bfdev_bintree_empty_root(&root->btree);
}

static __bfdev_always_inline void
bfdev_heap_clear_root(bfdev_heap_root_t *root)
{
    bfdev_bintree_clear_root(&root->btree);
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
bfdev_heap_find(bfdev_heap_root_t *root, unsigned int index);

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
    bool verify;

    verify = bfdev_bintree_link(&root->btree, parent, link, node);
    if (bfdev_unlikely(!verify))
        return;

    root->count++;
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
    bool verify;

    verify = bfdev_bintree_delete(node);
    if (bfdev_unlikely(!verify))
        return;

    rebalance = bfdev_heap_remove(root, node);
    if (rebalance)
        bfdev_heap_erase(root, rebalance, cmp, pdata);

    bfdev_bintree_clear(node);
}

/**
 * bfdev_heap_first_entry - get the preorder first element from a heap.
 * @root: the heap root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_first_entry(root, index, type, member) \
    bfdev_bintree_level_first_entry(&(root)->btree, index, type, member)

/**
 * bfdev_heap_next_entry - get the preorder next element in heap.
 * @root: the heap root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_next_entry(root, index, type, member) \
    bfdev_bintree_level_next_entry(&(root)->btree, index, type, member)

/**
 * bfdev_heap_for_each - preorder iterate over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @root: the root for your heap.
 */
#define bfdev_heap_for_each(pos, index, root) \
    bfdev_bintree_level_for_each(pos, index, &(root)->btree)

/**
 * bfdev_heap_for_each_from - preorder iterate over a heap from the current point.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 */
#define bfdev_heap_for_each_from(pos, index, root) \
    bfdev_bintree_level_for_each_from(pos, index, &(root)->btree)

/**
 * bfdev_heap_for_each_continue - continue preorder iteration over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 */
#define bfdev_heap_for_each_continue(pos, index, root) \
    bfdev_bintree_level_for_each_continue(pos, index, &(root)->btree)

/**
 * bfdev_heap_for_each_entry - preorder iterate over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your heap.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry(pos, index, root, member) \
    bfdev_bintree_level_for_each_entry(pos, index, &(root)->btree, member)

/**
 * bfdev_heap_for_each_entry_from - preorder iterate over heap of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry_from(pos, index, root, member) \
    bfdev_bintree_level_for_each_entry_from(pos, index, &(root)->btree, member)

/**
 * bfdev_heap_for_each_entry_continue - continue preorder iteration over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_for_each_entry_continue(pos, index, root, member) \
    bfdev_bintree_level_for_each_entry_continue(pos, index, &(root)->btree, member)

/**
 * bfdev_heap_post_first_entry - get the postorder first element from a heap.
 * @root: the heap root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_first_entry(root, type, member) \
    bfdev_bintree_post_first_entry(&(root)->btree, type, member)

/**
 * bfdev_heap_post_next_entry - get the postorder next element in heap.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_next_entry(pos, member) \
    bfdev_bintree_post_next_entry(pos, member)

/**
 * bfdev_heap_post_for_each - postorder iterate over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @root: the root for your heap.
 */
#define bfdev_heap_post_for_each(pos, root) \
    bfdev_bintree_post_for_each(pos, &(root)->btree)

/**
 * bfdev_heap_post_for_each_from - postorder iterate over a heap from the current point.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 */
#define bfdev_heap_post_for_each_from(pos) \
    bfdev_bintree_post_for_each_from(pos)

/**
 * bfdev_heap_post_for_each_continue - continue postorder iteration over a heap.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 */
#define bfdev_heap_post_for_each_continue(pos) \
    bfdev_bintree_post_for_each_continue(pos)

/**
 * bfdev_heap_post_for_each_safe - postorder iterate over a heap safe against removal of heap entry.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @tmp: another bfdev_heap_node to use as temporary storage.
 * @root: the root for your heap.
 */
#define bfdev_heap_post_for_each_safe(pos, tmp, root) \
    bfdev_bintree_post_for_each_safe(pos, tmp, &(root)->btree)

/**
 * bfdev_heap_post_for_each_safe_from - postorder iterate over a heap safe against removal of heap entry from the current point.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @tmp: another bfdev_heap_node to use as temporary storage.
 */
#define bfdev_heap_post_for_each_safe_from(pos, tmp) \
    bfdev_bintree_post_for_each_safe_from(pos, tmp)

/**
 * bfdev_heap_post_for_each_safe_continue - continue heap postorder iteration safe against removal.
 * @pos: the &bfdev_heap_node_t to use as a loop cursor.
 * @tmp: another bfdev_heap_node to use as temporary storage.
 */
#define bfdev_heap_post_for_each_safe_continue(pos, tmp) \
    bfdev_bintree_post_for_each_safe_continue(pos, tmp)

/**
 * bfdev_heap_post_for_each_entry - postorder iterate over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your heap.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry(pos, root, member) \
    bfdev_bintree_post_for_each_entry(pos, &(root)->btree, member)

/**
 * bfdev_heap_post_for_each_entry_from - postorder iterate over heap of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_from(pos, member) \
    bfdev_bintree_post_for_each_entry_from(pos, member)

/**
 * bfdev_heap_post_for_each_entry_continue - continue postorder iteration over heap of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_continue(pos, member) \
    bfdev_bintree_post_for_each_entry_continue(pos, member)

/**
 * bfdev_heap_post_for_each_entry_safe - postorder iterate over heap of given type safe against removal of heap entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @root: the root for your heap.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_safe(pos, tmp, root, member) \
    bfdev_bintree_post_for_each_entry_safe(pos, tmp, &(root)->btree, member)

/**
 * bfdev_heap_post_for_each_entry_from_safe - postorder iterate over heap from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_from_safe(pos, tmp, member) \
    bfdev_bintree_post_for_each_entry_from_safe(pos, tmp, member)

/**
 * bfdev_heap_post_for_each_entry_continue_safe - continue postorder heap iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_heap_node within the struct.
 */
#define bfdev_heap_post_for_each_entry_continue_safe(pos, tmp, member) \
    bfdev_bintree_post_for_each_entry_continue_safe(pos, tmp, member)

BFDEV_END_DECLS

#endif /* _BFDEV_HEAP_H_ */
