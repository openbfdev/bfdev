/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BINTREE_H_
#define _BFDEV_BINTREE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_bintree_root bfdev_bintree_root_t;
typedef struct bfdev_bintree_node bfdev_bintree_node_t;

struct bfdev_bintree_node {
    bfdev_bintree_node_t *parent;
    bfdev_bintree_node_t *left;
    bfdev_bintree_node_t *right;
};

struct bfdev_bintree_root {
    bfdev_bintree_node_t *node;
};

#define BFDEV_BINTREE_STATIC \
    {NULL}

#define BFDEV_BINTREE_INIT \
    (bfdev_bintree_root_t) BFDEV_BINTREE_STATIC

#define BFDEV_BINTREE_ROOT(name) \
    bfdev_bintree_root_t name = BFDEV_BINTREE_INIT

#define BFDEV_BINTREE_ROOT_NODE(root) \
    ((root)->node)

/**
 * bfdev_bintree_entry - get the struct for this entry.
 * @ptr: the &bfdev_bintree_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_bintree_entry_safe - get the struct for this entry or null.
 * @ptr: the &bfdev_bintree_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_entry_safe(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

static __bfdev_always_inline void
bfdev_bintree_init(bfdev_bintree_root_t *root)
{
    *root = BFDEV_BINTREE_INIT;
}

static __bfdev_always_inline bool
bfdev_bintree_empty_root(bfdev_bintree_root_t *root)
{
    return root->node == NULL;
}

static __bfdev_always_inline bool
bfdev_bintree_empty_node(bfdev_bintree_node_t *node)
{
    return node->parent == NULL;
}

static __bfdev_always_inline void
bfdev_bintree_clear_root(bfdev_bintree_root_t *root)
{
    root->node = NULL;
}

static __bfdev_always_inline void
bfdev_bintree_clear_node(bfdev_bintree_node_t *node)
{
    node->parent = NULL;
}

/**
 * bfdev_heap_link - link node to parent.
 * @root: heap root of node.
 * @parent: point to parent node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline bool
bfdev_bintree_link(bfdev_bintree_root_t *root, bfdev_bintree_node_t *parent,
                   bfdev_bintree_node_t **link, bfdev_bintree_node_t *node)
{
#ifdef BFDEV_DEBUG_BINTREE
    if (bfdev_unlikely(!bfdev_bintree_check_link(parent, link, node)))
        return false;
#endif

    /* link = &parent->left/right */
    *link = node;
    node->parent = parent;
    node->left = node->right = NULL;

    return true;
}

static inline bool
bfdev_bintree_delete(bfdev_bintree_node_t *node)
{
#ifdef BFDEV_DEBUG_BINTREE
    if (bfdev_unlikely(!bfdev_bintree_check_delete(node)))
        return false;
#endif

    return true;
}

static inline void
bfdev_bintree_clear(bfdev_bintree_node_t *node)
{
    node->left = BFDEV_POISON_HPNODE1;
    node->right = BFDEV_POISON_HPNODE2;
    node->parent = BFDEV_POISON_HPNODE3;
}

extern bool
bfdev_bintree_inherit(bfdev_bintree_root_t *root, bfdev_bintree_node_t *node);

/* Base iteration - basic iteration helper */
extern bfdev_bintree_node_t *
bfdev_bintree_left_far(const bfdev_bintree_node_t *node);

extern bfdev_bintree_node_t *
bfdev_bintree_right_far(const bfdev_bintree_node_t *node);

extern bfdev_bintree_node_t *
bfdev_bintree_left_deep(const bfdev_bintree_node_t *node);

extern bfdev_bintree_node_t *
bfdev_bintree_right_deep(const bfdev_bintree_node_t *node);

/* Inorder iteration (Sequential) - find logical next and previous nodes */
extern bfdev_bintree_node_t *
bfdev_bintree_first(const bfdev_bintree_root_t *root);

extern bfdev_bintree_node_t *
bfdev_bintree_last(const bfdev_bintree_root_t *root);

extern bfdev_bintree_node_t *
bfdev_bintree_prev(const bfdev_bintree_node_t *node);

extern bfdev_bintree_node_t *
bfdev_bintree_next(const bfdev_bintree_node_t *node);

/**
 * bfdev_bintree_first_entry - get the first element from a bintree.
 * @root: the bintree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_first_entry(root, type, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_first(root), type, member)

/**
 * bfdev_bintree_last_entry - get the last element from a bintree.
 * @root: the bintree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_last_entry(root, type, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_last(root), type, member)

/**
 * bfdev_bintree_next_entry - get the next element in bintree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_next_entry(pos, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_bintree_prev_entry - get the prev element in bintree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_prev_entry(pos, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_prev(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_bintree_for_each - iterate over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @root: the root for your bintree.
 */
#define bfdev_bintree_for_each(pos, root) \
    for (pos = bfdev_bintree_first(root); pos; pos = bfdev_bintree_next(pos))

/**
 * bfdev_bintree_for_each_reverse - iterate over a bintree backwards.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @root: the root for your bintree.
 */
#define bfdev_bintree_for_each_reverse(pos, root) \
    for (pos = bfdev_bintree_last(root); pos; pos = bfdev_bintree_prev(pos))

/**
 * bfdev_bintree_for_each_from - iterate over a bintree from the current point.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_for_each_from(pos) \
    for (; pos; pos = bfdev_bintree_next(pos))

/**
 * bfdev_bintree_for_each_reverse_from - iterate over a bintree backwards from the current point.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_for_each_reverse_from(pos) \
    for (; pos; pos = bfdev_bintree_prev(pos))

/**
 * bfdev_bintree_for_each_continue - continue iteration over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_for_each_continue(pos) \
    for (pos = bfdev_bintree_next(pos); pos; pos = bfdev_bintree_next(pos))

/**
 * bfdev_bintree_for_each_reverse_continue - continue iteration over a bintree backwards.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_for_each_reverse_continue(pos) \
    for (pos = bfdev_bintree_prev(pos); pos; pos = bfdev_bintree_prev(pos))

/**
 * bfdev_bintree_for_each_entry - iterate over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your bintree.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_for_each_entry(pos, root, member) \
    for (pos = bfdev_bintree_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_bintree_next_entry(pos, member))

/**
 * bfdev_bintree_for_each_entry_reverse - iterate backwards over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your bintree.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_for_each_entry_reverse(pos, root, member) \
    for (pos = bfdev_bintree_last_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_bintree_prev_entry(pos, member))

/**
 * bfdev_bintree_for_each_entry_from - iterate over bintree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_bintree_next_entry(pos, member))

/**
 * bfdev_bintree_for_each_entry_reverse_from - iterate backwards over bintree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_for_each_entry_reverse_from(pos, member) \
    for (; pos; pos = bfdev_bintree_prev_entry(pos, member))

/**
 * bfdev_bintree_for_each_entry_continue - continue iteration over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_for_each_entry_continue(pos, member) \
    for (pos = bfdev_bintree_next_entry(pos, member); pos; \
         pos = bfdev_bintree_next_entry(pos, member))

/**
 * bfdev_bintree_for_each_entry_reverse_continue - iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_for_each_entry_reverse_continue(pos, member) \
    for (pos = bfdev_bintree_prev_entry(pos, member); pos; \
         pos = bfdev_bintree_prev_entry(pos, member))

/* Preorder iteration (Root-first) - always access the left node first */
extern bfdev_bintree_node_t *
bfdev_bintree_pre_first(const bfdev_bintree_root_t *root);

extern bfdev_bintree_node_t *
bfdev_bintree_pre_next(const bfdev_bintree_node_t *node);

/**
 * bfdev_bintree_pre_first_entry - get the preorder first element from a bintree.
 * @root: the bintree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_pre_first_entry(root, type, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_pre_first(root), type, member)

/**
 * bfdev_bintree_pre_next_entry - get the preorder next element in bintree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_pre_next_entry(pos, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_pre_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_bintree_pre_for_each - preorder iterate over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @root: the root for your bintree.
 */
#define bfdev_bintree_pre_for_each(pos, root) \
    for (pos = bfdev_bintree_pre_first(root); pos; pos = bfdev_bintree_pre_next(pos))

/**
 * bfdev_bintree_pre_for_each_from - preorder iterate over a bintree from the current point.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_pre_for_each_from(pos) \
    for (; pos; pos = bfdev_bintree_pre_next(pos))

/**
 * bfdev_bintree_pre_for_each_continue - continue preorder iteration over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_pre_for_each_continue(pos) \
    for (pos = bfdev_bintree_pre_next(pos); pos; pos = bfdev_bintree_pre_next(pos))

/**
 * bfdev_bintree_pre_for_each_entry - preorder iterate over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your bintree.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_pre_for_each_entry(pos, root, member) \
    for (pos = bfdev_bintree_pre_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_bintree_pre_next_entry(pos, member))

/**
 * bfdev_bintree_pre_for_each_entry_from - preorder iterate over bintree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_pre_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_bintree_pre_next_entry(pos, member))

/**
 * bfdev_bintree_pre_for_each_entry_continue - continue preorder iteration over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_pre_for_each_entry_continue(pos, member) \
    for (pos = bfdev_bintree_pre_next_entry(pos, member); \
         pos; pos = bfdev_bintree_pre_next_entry(pos, member))

/* Postorder iteration (Depth-first) - always visit the parent after its children */
extern bfdev_bintree_node_t *
bfdev_bintree_post_first(const bfdev_bintree_root_t *root);

extern bfdev_bintree_node_t *
bfdev_bintree_post_next(const bfdev_bintree_node_t *node);

/**
 * bfdev_bintree_post_first_entry - get the postorder first element from a bintree.
 * @root: the bintree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_first_entry(root, type, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_post_first(root), type, member)

/**
 * bfdev_bintree_post_next_entry - get the postorder next element in bintree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_next_entry(pos, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_post_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_bintree_post_for_each - postorder iterate over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @root: the root for your bintree.
 */
#define bfdev_bintree_post_for_each(pos, root) \
    for (pos = bfdev_bintree_post_first(root); pos; pos = bfdev_bintree_post_next(pos))

/**
 * bfdev_bintree_post_for_each_from - postorder iterate over a bintree from the current point.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_post_for_each_from(pos) \
    for (; pos; pos = bfdev_bintree_post_next(pos))

/**
 * bfdev_bintree_post_for_each_continue - continue postorder iteration over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_post_for_each_continue(pos) \
    for (pos = bfdev_bintree_post_next(pos); pos; pos = bfdev_bintree_post_next(pos))

/**
 * bfdev_bintree_post_for_each_safe - postorder iterate over a bintree safe against removal of bintree entry.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @tmp: another bfdev_bintree_node to use as temporary storage.
 * @root: the root for your bintree.
 */
#define bfdev_bintree_post_for_each_safe(pos, tmp, root) \
    for (pos = bfdev_bintree_post_first(root); pos && \
        ({tmp = bfdev_bintree_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_bintree_post_for_each_safe_from - postorder iterate over a bintree safe against removal of bintree entry from the current point.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @tmp: another bfdev_bintree_node to use as temporary storage.
 */
#define bfdev_bintree_post_for_each_safe_from(pos, tmp) \
    for (; pos && ({tmp = bfdev_bintree_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_bintree_post_for_each_safe_continue - continue bintree postorder iteration safe against removal.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @tmp: another bfdev_bintree_node to use as temporary storage.
 */
#define bfdev_bintree_post_for_each_safe_continue(pos, tmp) \
    for (pos = bfdev_bintree_post_next(pos); pos && \
        ({tmp = bfdev_bintree_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_bintree_post_for_each_entry - postorder iterate over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your bintree.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_for_each_entry(pos, root, member) \
    for (pos = bfdev_bintree_post_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_bintree_post_next_entry(pos, member))

/**
 * bfdev_bintree_post_for_each_entry_from - postorder iterate over bintree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_bintree_post_next_entry(pos, member))

/**
 * bfdev_bintree_post_for_each_entry_continue - continue postorder iteration over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_for_each_entry_continue(pos, member) \
    for (pos = bfdev_bintree_post_next_entry(pos, member); \
         pos; pos = bfdev_bintree_post_next_entry(pos, member))

/**
 * bfdev_bintree_post_for_each_entry_safe - postorder iterate over bintree of given type safe against removal of bintree entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @root: the root for your bintree.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_for_each_entry_safe(pos, tmp, root, member) \
    for (pos = bfdev_bintree_post_first_entry(root, typeof(*pos), member); \
         pos && ({ tmp = bfdev_bintree_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_bintree_post_for_each_entry_from_safe - postorder iterate over bintree from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_for_each_entry_from_safe(pos, tmp, member) \
    for (; pos && ({ tmp = bfdev_bintree_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_bintree_post_for_each_entry_continue_safe - continue postorder bintree iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_post_for_each_entry_continue_safe(pos, tmp, member) \
    for (pos = bfdev_bintree_post_next_entry(pos, member); \
         pos && ({ tmp = bfdev_bintree_post_next_entry(pos, member); \
         1; }); pos = tmp)

/* Level iteration (Sequential) - access in level sequence */
extern bfdev_bintree_node_t *
bfdev_bintree_level_first(const bfdev_bintree_root_t *root, unsigned long *index);

extern bfdev_bintree_node_t *
bfdev_bintree_level_next(const bfdev_bintree_root_t *root, unsigned long *index);

/**
 * bfdev_bintree_level_first_entry - get the preorder first element from a bintree.
 * @root: the bintree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_level_first_entry(root, index, type, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_level_first(root, index), type, member)

/**
 * bfdev_bintree_level_next_entry - get the preorder next element in bintree.
 * @root: the bintree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_level_next_entry(root, index, type, member) \
    bfdev_bintree_entry_safe(bfdev_bintree_level_next(root, index), type, member)

/**
 * bfdev_bintree_level_for_each - preorder iterate over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 * @root: the root for your bintree.
 */
#define bfdev_bintree_level_for_each(pos, index, root) \
    for (pos = bfdev_bintree_level_first(root, index); \
         pos; pos = bfdev_bintree_level_next(root, index))

/**
 * bfdev_bintree_level_for_each_from - preorder iterate over a bintree from the current point.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_level_for_each_from(pos, index, root) \
    for (; pos; pos = bfdev_bintree_level_next(root, index))

/**
 * bfdev_bintree_level_for_each_continue - continue preorder iteration over a bintree.
 * @pos: the &bfdev_bintree_node_t to use as a loop cursor.
 */
#define bfdev_bintree_level_for_each_continue(pos, index, root) \
    for (pos = bfdev_bintree_level_next(root, index); \
         pos; pos = bfdev_bintree_level_next(root, index))

/**
 * bfdev_bintree_level_for_each_entry - preorder iterate over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your bintree.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_level_for_each_entry(pos, index, root, member) \
    for (pos = bfdev_bintree_level_first_entry(root, index, typeof(*pos), member); \
         pos; pos = bfdev_bintree_level_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_bintree_level_for_each_entry_from - preorder iterate over bintree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_level_for_each_entry_from(pos, index, root, member) \
    for (; pos; pos = bfdev_bintree_level_next_entry(root, index, typeof(*pos), member))

/**
 * bfdev_bintree_level_for_each_entry_continue - continue preorder iteration over bintree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_bintree_node within the struct.
 */
#define bfdev_bintree_level_for_each_entry_continue(pos, index, root, member) \
    for (pos = bfdev_bintree_level_next_entry(root, index, typeof(*pos), member); \
         pos; pos = bfdev_bintree_level_next_entry(root, index, typeof(*pos), member))

BFDEV_END_DECLS

#endif /* _BFDEV_BINTREE_H_ */
