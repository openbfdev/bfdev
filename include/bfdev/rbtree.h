/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RBTREE_H_
#define _BFDEV_RBTREE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/limits.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

#define BFDEV_RB_RED    (0)
#define BFDEV_RB_BLACK  (1)
#define BFDEV_RB_NSET   (2)

typedef struct bfdev_rb_node bfdev_rb_node_t;
typedef struct bfdev_rb_root bfdev_rb_root_t;
typedef struct bfdev_rb_root_cached bfdev_rb_root_cached_t;

struct bfdev_rb_node {
    bfdev_rb_node_t *parent;
    bfdev_rb_node_t *left;
    bfdev_rb_node_t *right;
    bool color;
};

struct bfdev_rb_root {
    bfdev_rb_node_t *node;
};

struct bfdev_rb_root_cached {
    bfdev_rb_root_t root;
    bfdev_rb_node_t *leftmost;
};

struct bfdev_rb_callbacks {
    void (*rotate)(bfdev_rb_node_t *node, bfdev_rb_node_t *successor);
    void (*copy)(bfdev_rb_node_t *node, bfdev_rb_node_t *successor);
    void (*propagate)(bfdev_rb_node_t *node, bfdev_rb_node_t *stop);
};

#define BFDEV_RB_STATIC() { \
    .node = NULL, \
}

#define BFDEV_RB_CACHED_STATIC() { \
    .root = BFDEV_RB_STATIC(), \
    .leftmost = NULL, \
}

#define BFDEV_RB_INIT() \
    (bfdev_rb_root_t) BFDEV_RB_STATIC()

#define BFDEV_RB_CACHED_INIT() \
    (bfdev_rb_root_cached_t) BFDEV_RB_CACHED_STATIC()

#define BFDEV_RB_ROOT(name) \
    bfdev_rb_root_t name = BFDEV_RB_INIT()

#define BFDEV_RB_ROOT_CACHED(name) \
    bfdev_rb_root_cached_t name = BFDEV_RB_CACHED_INIT()

#define BFDEV_RB_EMPTY_ROOT(root) \
    ((root)->node == NULL)

#define BFDEV_RB_EMPTY_ROOT_CACHED(cached) \
    ((cached)->root.node == NULL)

/**
 * bfdev_rb_entry - get the struct for this entry.
 * @ptr: the &bfdev_rb_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_rb_entry_safe - get the struct for this entry or null.
 * @ptr: the &bfdev_rb_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_entry_safe(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

#ifdef BFDEV_DEBUG_RBTREE
extern bool
bfdev_rb_check_link(bfdev_rb_node_t *parent, bfdev_rb_node_t **link,
                    bfdev_rb_node_t *node);

extern bool
bfdev_rb_check_delete(bfdev_rb_node_t *node);
#endif

BFDEV_CALLBACK_FIND(
    bfdev_rb_find_t,
    const bfdev_rb_node_t *
);

BFDEV_CALLBACK_CMP(
    bfdev_rb_cmp_t,
    const bfdev_rb_node_t *
);

static inline void
bfdev_rb_init(bfdev_rb_root_t *root)
{
    *root = BFDEV_RB_INIT();
}

static inline void
bfdev_rb_cache_init(bfdev_rb_root_cached_t *root)
{
    *root = BFDEV_RB_CACHED_INIT();
}

/**
 * bfdev_rb_fixup_augmented() - augmented balance after insert node.
 * @root: rbtree root of node.
 * @node: new inserted node.
 * @callbacks: augmented callback function.
 */
extern void
bfdev_rb_fixup_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                         const struct bfdev_rb_callbacks *callbacks);

/**
 * bfdev_rb_erase_augmented() - augmented balance after remove node.
 * @root: rbtree root of node.
 * @parent: parent of removed node.
 * @callbacks: augmented callback function.
 */
extern void
bfdev_rb_erase_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *parent,
                         const struct bfdev_rb_callbacks *callbacks);

/**
 * bfdev_rb_remove_augmented() - augmented remove node form rbtree.
 * @root: rbtree root of node.
 * @node: node to remove.
 * @callbacks: augmented callback function.
 */
extern bfdev_rb_node_t *
bfdev_rb_remove_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                          const struct bfdev_rb_callbacks *callbacks);

/**
 * bfdev_rb_fixup() - balance after insert node.
 * @root: rbtree root of node.
 * @node: new inserted node.
 */
extern void
bfdev_rb_fixup(bfdev_rb_root_t *root, bfdev_rb_node_t *node);

/**
 * bfdev_rb_erase() - balance after remove node.
 * @root: rbtree root of node.
 * @parent: parent of removed node.
 */
extern void
bfdev_rb_erase(bfdev_rb_root_t *root, bfdev_rb_node_t *parent);

/**
 * bfdev_rb_remove() - remove node form rbtree.
 * @root: rbtree root of node.
 * @node: node to remove.
 */
extern bfdev_rb_node_t *
bfdev_rb_remove(bfdev_rb_root_t *root, bfdev_rb_node_t *node);

/**
 * bfdev_rb_replace() - replace old node by new one.
 * @root: rbtree root of node.
 * @oldn: node to be replaced.
 * @newn: new node to insert.
 */
extern void
bfdev_rb_replace(bfdev_rb_root_t *root, bfdev_rb_node_t *oldn,
                 bfdev_rb_node_t *newn);

/**
 * bfdev_rb_find() - find @key in tree @root.
 * @root: rbtree want to search.
 * @key: key to match.
 * @cmp: operator defining the node order.
 */
extern bfdev_rb_node_t *
bfdev_rb_find(const bfdev_rb_root_t *root, void *key, bfdev_rb_find_t cmp);

/**
 * bfdev_rb_find_last() - find @key in tree @root and return parent.
 * @root: rbtree want to search.
 * @key: key to match.
 * @cmp: operator defining the node order.
 * @parentp: pointer used to modify the parent node pointer.
 * @linkp: pointer used to modify the point to pointer to child node.
 */
extern bfdev_rb_node_t *
bfdev_rb_find_last(bfdev_rb_root_t *root, void *key, bfdev_rb_find_t cmp,
                   bfdev_rb_node_t **parentp, bfdev_rb_node_t ***linkp);

/**
 * bfdev_rb_parent() - find the parent node.
 * @root: rbtree want to search.
 * @parentp: pointer used to modify the parent node pointer.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 * @leftmostp: return whether it is the leftmost node.
 */
extern bfdev_rb_node_t **
bfdev_rb_parent(bfdev_rb_root_t *root, bfdev_rb_node_t **parentp,
                bfdev_rb_node_t *node, bfdev_rb_cmp_t cmp, void *pdata,
                bool *leftmostp);

#define bfdev_rb_cached_erase_augmented(cached, parent, callbacks) \
    bfdev_rb_erase_augmented(&(cached)->root, parent, callbacks)

#define bfdev_rb_cached_remove_augmentebfdev_rb_pre_nextd(cached, node, callbacks) \
    bfdev_rb_remove_augmented(&(cached)->root, node, callbacks)

#define bfdev_rb_cached_erase(cached, parent) \
    bfdev_rb_erase(&(cached)->root, parent)

#define bfdev_rb_cached_remove(cached, node) \
    bfdev_rb_remove(&(cached)->root, node)

#define bfdev_rb_cached_find(cached, key, cmp) \
    bfdev_rb_find(&(cached)->root, key, cmp)

#define bfdev_rb_cached_find_last(cached, key, cmp, parentp, linkp) \
    bfdev_rb_find_last(&(cached)->root, key, cmp, parentp, linkp)

#define bfdev_rb_cached_parent(cached, parentp, node, cmp, pdata, leftmostp) \
    bfdev_rb_parent(&(cached)->root, parentp, node, cmp, pdata, leftmostp)

/* Base iteration - basic iteration helper */
extern bfdev_rb_node_t *
bfdev_rb_left_far(const bfdev_rb_node_t *node);

extern bfdev_rb_node_t *
bfdev_rb_right_far(const bfdev_rb_node_t *node);

extern bfdev_rb_node_t *
bfdev_rb_left_deep(const bfdev_rb_node_t *node);

extern bfdev_rb_node_t *
bfdev_rb_right_deep(const bfdev_rb_node_t *node);

/* Inorder iteration (Sequential) - find logical next and previous nodes */
extern bfdev_rb_node_t *
bfdev_rb_first(const bfdev_rb_root_t *root);

extern bfdev_rb_node_t *
bfdev_rb_last(const bfdev_rb_root_t *root);

extern bfdev_rb_node_t *
bfdev_rb_prev(const bfdev_rb_node_t *node);

extern bfdev_rb_node_t *
bfdev_rb_next(const bfdev_rb_node_t *node);

/* Preorder iteration (Root-first) - always access the left node first */
extern bfdev_rb_node_t *
bfdev_rb_pre_first(const bfdev_rb_root_t *root);

extern bfdev_rb_node_t *
bfdev_rb_pre_next(const bfdev_rb_node_t *node);

/* Postorder iteration (Depth-first) - always visit the parent after its children */
extern bfdev_rb_node_t *
bfdev_rb_post_first(const bfdev_rb_root_t *root);

extern bfdev_rb_node_t *
bfdev_rb_post_next(const bfdev_rb_node_t *node);

/**
 * bfdev_rb_first_entry - get the first element from a rbtree.
 * @ptr: the rbtree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_first_entry(ptr, type, member) \
    bfdev_rb_entry_safe(bfdev_rb_first(ptr), type, member)

/**
 * bfdev_rb_last_entry - get the last element from a rbtree.
 * @ptr: the rbtree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_last_entry(ptr, type, member) \
    bfdev_rb_entry_safe(bfdev_rb_last(ptr), type, member)

/**
 * bfdev_rb_next_entry - get the next element in rbtree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_next_entry(pos, member) \
    bfdev_rb_entry_safe(bfdev_rb_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_rb_prev_entry - get the prev element in rbtree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_prev_entry(pos, member) \
    bfdev_rb_entry_safe(bfdev_rb_prev(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_rb_for_each - iterate over a rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_for_each(pos, root) \
    for (pos = bfdev_rb_first(root); pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_for_each_reverse - iterate over a rbtree backwards.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_for_each_reverse(pos, root) \
    for (pos = bfdev_rb_last(root); pos; pos = bfdev_rb_prev(pos))

/**
 * bfdev_rb_for_each_from - iterate over a rbtree from the current point.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_for_each_from(pos) \
    for (; pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_for_each_reverse_from - iterate over a rbtree backwards from the current point.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_for_each_reverse_from(pos) \
    for (; pos; pos = bfdev_rb_prev(pos))

/**
 * bfdev_rb_for_each_continue - continue iteration over a rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_for_each_continue(pos) \
    for (pos = bfdev_rb_next(pos); pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_for_each_reverse_continue - continue iteration over a rbtree backwards.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_for_each_reverse_continue(pos) \
    for (pos = bfdev_rb_prev(pos); pos; pos = bfdev_rb_prev(pos))

/**
 * bfdev_rb_for_each_entry - iterate over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_for_each_entry(pos, root, member) \
    for (pos = bfdev_rb_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_rb_next_entry(pos, member))

/**
 * bfdev_rb_for_each_entry_reverse - iterate backwards over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_for_each_entry_reverse(pos, root, member) \
    for (pos = bfdev_rb_last_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_rb_prev_entry(pos, member))

/**
 * bfdev_rb_for_each_entry_from - iterate over rbtree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_rb_next_entry(pos, member))

/**
 * bfdev_rb_for_each_entry_reverse_from - iterate backwards over rbtree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_for_each_entry_reverse_from(pos, member) \
    for (; pos; pos = bfdev_rb_prev_entry(pos, member))

/**
 * bfdev_rb_for_each_entry_continue - continue iteration over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_for_each_entry_continue(pos, member) \
    for (pos = bfdev_rb_next_entry(pos, member); pos; \
         pos = bfdev_rb_next_entry(pos, member))

/**
 * bfdev_rb_for_each_entry_reverse_continue - iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_for_each_entry_reverse_continue(pos, member) \
    for (pos = bfdev_rb_prev_entry(pos, member); pos; \
         pos = bfdev_rb_prev_entry(pos, member))

/**
 * bfdev_rb_pre_first_entry - get the preorder first element from a rbtree.
 * @ptr: the rbtree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_pre_first_entry(root, type, member) \
    bfdev_rb_entry_safe(bfdev_rb_pre_first(root), type, member)

/**
 * bfdev_rb_pre_next_entry - get the preorder next element in rbtree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_pre_next_entry(pos, member) \
    bfdev_rb_entry_safe(bfdev_rb_pre_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_rb_pre_for_each - preorder iterate over a rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_pre_for_each(pos, root) \
    for (pos = bfdev_rb_pre_first(root); pos; pos = bfdev_rb_pre_next(pos))

/**
 * bfdev_rb_pre_for_each_from - preorder iterate over a rbtree from the current point.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_pre_for_each_from(pos) \
    for (; pos; pos = bfdev_rb_pre_next(pos))

/**
 * bfdev_rb_pre_for_each_continue - continue preorder iteration over a rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_pre_for_each_continue(pos) \
    for (pos = bfdev_rb_pre_next(pos); pos; pos = bfdev_rb_pre_next(pos))

/**
 * bfdev_rb_pre_for_each_entry - preorder iterate over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_pre_for_each_entry(pos, root, member) \
    for (pos = bfdev_rb_pre_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_rb_pre_next_entry(pos, member))

/**
 * bfdev_rb_pre_for_each_entry_from - preorder iterate over rbtree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_pre_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_rb_pre_next_entry(pos, member))

/**
 * bfdev_rb_pre_for_each_entry_continue - continue preorder iteration over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_pre_for_each_entry_continue(pos, member) \
    for (pos = bfdev_rb_pre_next_entry(pos, member); \
         pos; pos = bfdev_rb_pre_next_entry(pos, member))

/**
 * bfdev_rb_post_first_entry - get the postorder first element from a rbtree.
 * @ptr: the rbtree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_first_entry(ptr, type, member) \
    bfdev_rb_entry_safe(bfdev_rb_post_first(ptr), type, member)

/**
 * bfdev_rb_post_next_entry - get the postorder next element in rbtree.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_next_entry(pos, member) \
    bfdev_rb_entry_safe(bfdev_rb_post_next(&(pos)->member), typeof(*(pos)), member)

/**
 * bfdev_rb_post_for_each - postorder iterate over a rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_post_for_each(pos, root) \
    for (pos = bfdev_rb_post_first(root); pos; pos = bfdev_rb_post_next(pos))

/**
 * bfdev_rb_post_for_each_from - postorder iterate over a rbtree from the current point.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_post_for_each_from(pos) \
    for (; pos; pos = bfdev_rb_post_next(pos))

/**
 * bfdev_rb_post_for_each_continue - continue postorder iteration over a rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 */
#define bfdev_rb_post_for_each_continue(pos) \
    for (pos = bfdev_rb_post_next(pos); pos; pos = bfdev_rb_post_next(pos))

/**
 * bfdev_rb_post_for_each_safe - postorder iterate over a rbtree safe against removal of rbtree entry.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @tmp: another bfdev_rb_node to use as temporary storage.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_post_for_each_safe(pos, tmp, root) \
    for (pos = bfdev_rb_post_first(root); pos && \
        ({tmp = bfdev_rb_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_safe_from - postorder iterate over a rbtree safe against removal of rbtree entry from the current point.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @tmp: another bfdev_rb_node to use as temporary storage.
 */
#define bfdev_rb_post_for_each_safe_from(pos, tmp) \
    for (; pos && ({tmp = bfdev_rb_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_safe_continue - continue rbtree postorder iteration safe against removal.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @tmp: another bfdev_rb_node to use as temporary storage.
 */
#define bfdev_rb_post_for_each_safe_continue(pos, tmp) \
    for (pos = bfdev_rb_post_next(pos); pos && \
        ({tmp = bfdev_rb_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_entry - postorder iterate over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @root: the root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_for_each_entry(pos, root, member) \
    for (pos = bfdev_rb_post_first_entry(root, typeof(*pos), member); \
         pos; pos = bfdev_rb_post_next_entry(pos, member))

/**
 * bfdev_rb_post_for_each_entry_from - postorder iterate over rbtree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_for_each_entry_from(pos, member) \
    for (; pos; pos = bfdev_rb_post_next_entry(pos, member))

/**
 * bfdev_rb_post_for_each_entry_continue - continue postorder iteration over rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_for_each_entry_continue(pos, member) \
    for (pos = bfdev_rb_post_next_entry(pos, member); \
         pos; pos = bfdev_rb_post_next_entry(pos, member))

/**
 * bfdev_rb_post_for_each_entry_safe - postorder iterate over rbtree of given type safe against removal of rbtree entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @root: the root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_for_each_entry_safe(pos, tmp, root, member) \
    for (pos = bfdev_rb_post_first_entry(root, typeof(*pos), member); \
         pos && ({ tmp = bfdev_rb_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_entry_from_safe - postorder iterate over rbtree from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_for_each_entry_from_safe(pos, tmp, member) \
    for (; pos && ({ tmp = bfdev_rb_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_entry_continue_safe - continue postorder rbtree iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_post_for_each_entry_continue_safe(pos, tmp, member) \
    for (pos = bfdev_rb_post_next_entry(pos, member); \
         pos && ({ tmp = bfdev_rb_post_next_entry(pos, member); \
         1; }); pos = tmp)

/**
 * bfdev_rb_link - link node to parent.
 * @parent: point to parent node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void
bfdev_rb_link(bfdev_rb_node_t *parent, bfdev_rb_node_t **link, bfdev_rb_node_t *node)
{
#ifdef BFDEV_DEBUG_RBTREE
    if (bfdev_unlikely(!bfdev_rb_check_link(parent, link, node)))
        return;
#endif

    /* link = &parent->left/right */
    *link = node;
    node->parent = parent;
    node->color = BFDEV_RB_RED;
    node->left = node->right = NULL;
}

/**
 * bfdev_rb_insert_node - link node to parent and fixup rbtree.
 * @root: rbtree root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void
bfdev_rb_insert_node(bfdev_rb_root_t *root, bfdev_rb_node_t *parent,
                     bfdev_rb_node_t **link, bfdev_rb_node_t *node)
{
    bfdev_rb_link(parent, link, node);
    bfdev_rb_fixup(root, node);
}

/**
 * bfdev_rb_insert - find the parent node and insert new node.
 * @root: rbtree root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline void
bfdev_rb_insert(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                bfdev_rb_cmp_t cmp, void *pdata)
{
    bfdev_rb_node_t *parent, **link;

    link = bfdev_rb_parent(root, &parent, node, cmp, pdata, NULL);
    bfdev_rb_insert_node(root, parent, link, node);
}

/**
 * bfdev_rb_delete - delete node and fixup rbtree.
 * @root: rbtree root of node.
 * @node: node to delete.
 */
static inline void
bfdev_rb_delete(bfdev_rb_root_t *root, bfdev_rb_node_t *node)
{
    bfdev_rb_node_t *rebalance;

#ifdef BFDEV_DEBUG_RBTREE
    if (bfdev_unlikely(!bfdev_rb_check_delete(node)))
        return;
#endif

    if ((rebalance = bfdev_rb_remove(root, node)))
        bfdev_rb_erase(root, rebalance);

    node->left = BFDEV_POISON_RBNODE1;
    node->right = BFDEV_POISON_RBNODE2;
    node->parent = BFDEV_POISON_RBNODE3;
}

/**
 * bfdev_rb_insert_node_augmented - augmented link node to parent and fixup rbtree.
 * @root: rbtree root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_insert_node_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *parent,
                               bfdev_rb_node_t **link, bfdev_rb_node_t *node,
                               const struct bfdev_rb_callbacks *callbacks)
{
    bfdev_rb_link(parent, link, node);
    bfdev_rb_fixup_augmented(root, node, callbacks);
}

/**
 * bfdev_rb_insert_augmented - augmented find the parent node and insert new node.
 * @root: rbtree root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_insert_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                          bfdev_rb_cmp_t cmp, void *pdata,
                          const struct bfdev_rb_callbacks *callbacks)
{
    bfdev_rb_node_t *parent, **link;

    link = bfdev_rb_parent(root, &parent, node, cmp, pdata, NULL);
    bfdev_rb_insert_node_augmented(root, parent, link, node, callbacks);
}

/**
 * bfdev_rb_delete_augmented - augmented delete node and fixup rbtree.
 * @root: rbtree root of node.
 * @node: node to delete.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_delete_augmented(bfdev_rb_root_t *root, bfdev_rb_node_t *node,
                          const struct bfdev_rb_callbacks *callbacks)
{
    bfdev_rb_node_t *rebalance;

#ifdef BFDEV_DEBUG_RBTREE
    if (bfdev_unlikely(!bfdev_rb_check_delete(node)))
        return;
#endif

    if ((rebalance = bfdev_rb_remove_augmented(root, node, callbacks)))
        bfdev_rb_erase_augmented(root, rebalance, callbacks);

    node->left = BFDEV_POISON_RBNODE1;
    node->right = BFDEV_POISON_RBNODE2;
    node->parent = BFDEV_POISON_RBNODE3;
}

/**
 * bfdev_rb_cached_first - get the first bfdev_rb_node from a cached rbtree.
 * @cached: the rbtree root to take the bfdev_rb_node from.
 */
#define bfdev_rb_cached_first(cached) \
    ((cached)->leftmost)

/**
 * bfdev_rb_cached_first_entry - get the first element from a cached rbtree.
 * @ptr: the rbtree root to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_cached_first_entry(ptr, type, member) \
    bfdev_rb_entry_safe(bfdev_rb_cached_first(ptr), type, member)

/**
 * bfdev_rb_cached_for_each - iterate over a cached rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_for_each(pos, cached) \
    for (pos = bfdev_rb_cached_first(cached); pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_cached_for_each_reverse - iterate over a cached rbtree backwards.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_for_each_reverse(pos, cached) \
    bfdev_rb_for_each_reverse(pos, &(cached)->root)

/**
 * bfdev_rb_cached_for_each_entry - iterate over cached rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_cached_for_each_entry(pos, cached, member) \
    for (pos = bfdev_rb_cached_first_entry(cached, typeof(*pos), member); \
         pos; pos = bfdev_rb_next_entry(pos, member))

/**
 * bfdev_rb_cached_for_each_entry_reverse - iterate backwards over cached rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_cached_for_each_entry_reverse(pos, cached, member) \
    bfdev_rb_for_each_entry_reverse(pos, &(cached)->root, member)

/**
 * bfdev_rb_cached_pre_for_each - preorder iterate over a cached rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_pre_for_each(pos, cached) \
    bfdev_rb_pre_for_each(pos, &(cached)->root)

/**
 * bfdev_rb_cached_pre_for_each_entry - preorder iterate over cached rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_cached_pre_for_each_entry(pos, cached, member) \
    bfdev_rb_pre_for_each_entry(pos, &(cached)->root, member)

/**
 * bfdev_rb_cached_post_for_each - postorder iterate over a cached rbtree.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_post_for_each(pos, cached) \
    bfdev_rb_post_for_each(pos, &(cached)->root)

/**
 * bfdev_rb_cached_post_for_each_safe - postorder iterate over a cached rbtree safe against removal of rbtree entry.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @tmp: another bfdev_rb_node to use as temporary storage.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_post_for_each_safe(pos, tmp, cached) \
    bfdev_rb_post_for_each_safe(pos, tmp, &(cached)->root)

/**
 * bfdev_rb_cached_post_for_each_entry - postorder iterate over cached rbtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_cached_post_for_each_entry(pos, cached, member) \
    bfdev_rb_post_for_each_entry(pos, &(cached)->root, member)

/**
 * bfdev_rb_cached_post_for_each_entry_safe - postorder iterate over cached rbtree of given type safe against removal of rbtree entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @cached: the cached root for your rbtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_cached_post_for_each_entry_safe(pos, tmp, cached, member) \
    bfdev_rb_post_for_each_entry_safe(pos, tmp, &(cached)->root, member)

/**
 * bfdev_rb_cached_fixup - balance after insert cached node.
 * @cached: rbtree cached root of node.
 * @node: new inserted node.
 * @leftmost: is it the leftmost node.
 */
static inline void
bfdev_rb_cached_fixup(bfdev_rb_root_cached_t *cached,
                      bfdev_rb_node_t *node, bool leftmost)
{
    if (leftmost)
        cached->leftmost = node;

    bfdev_rb_fixup(&cached->root, node);
}

/**
 * bfdev_rb_cached_insert_node - link cached node to parent and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 * @leftmost: is it the leftmost node.
 */
static inline void
bfdev_rb_cached_insert_node(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *parent,
                            bfdev_rb_node_t **link, bfdev_rb_node_t *node, bool leftmost)
{
    bfdev_rb_link(parent, link, node);
    bfdev_rb_cached_fixup(cached, node, leftmost);
}

/**
 * bfdev_rb_cached_insert - find the parent node and insert new cached node.
 * @cached: rbtree cached root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline void
bfdev_rb_cached_insert(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *node,
                       bfdev_rb_cmp_t cmp, void *pdata)
{
    bfdev_rb_node_t *parent, **link;
    bool leftmost;

    link = bfdev_rb_cached_parent(cached, &parent, node, cmp, pdata, &leftmost);
    bfdev_rb_cached_insert_node(cached, parent, link, node, leftmost);
}

/**
 * bfdev_rb_cached_delete - delete cached node and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @node: node to delete.
 */
static inline bfdev_rb_node_t *
bfdev_rb_cached_delete(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *node)
{
    bfdev_rb_node_t *leftmost;

    if (cached->leftmost != node)
        leftmost = NULL;
    else {
        leftmost = bfdev_rb_next(node);
        cached->leftmost = leftmost;
    }

    bfdev_rb_delete(&cached->root, node);
    return leftmost;
}

/**
 * bfdev_rb_cached_fixup_augmented - augmented balance after insert cached node.
 * @cached: rbtree cached root of node.
 * @node: new inserted node.
 * @leftmost: is it the leftmost node.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_cached_fixup_augmented(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *node,
                                bool leftmost, const struct bfdev_rb_callbacks *callbacks)
{
    if (leftmost)
        cached->leftmost = node;

    bfdev_rb_fixup_augmented(&cached->root, node, callbacks);
}

/**
 * bfdev_rb_cached_insert_node - augmented link cached node to parent and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 * @leftmost: is it the leftmost node.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_cached_insert_node_augmented(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *parent,
                                      bfdev_rb_node_t **link, bfdev_rb_node_t *node,
                                      bool leftmost, const struct bfdev_rb_callbacks *callbacks)
{
    bfdev_rb_link(parent, link, node);
    bfdev_rb_cached_fixup_augmented(cached, node, leftmost, callbacks);
}

/**
 * bfdev_rb_cached_insert - augmented find the parent node and insert new cached node.
 * @cached: rbtree cached root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_cached_insert_augmented(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *node,
                                 bfdev_rb_cmp_t cmp, void *pdata,
                                 const struct bfdev_rb_callbacks *callbacks)
{
    bfdev_rb_node_t *parent, **link;
    bool leftmost;

    link = bfdev_rb_cached_parent(cached, &parent, node, cmp, pdata, &leftmost);
    bfdev_rb_cached_insert_node_augmented(cached, parent, link, node, leftmost, callbacks);
}

/**
 * bfdev_rb_cached_delete - delete cached node and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @node: node to delete.
 * @callbacks: augmented callback function.
 */
static inline bfdev_rb_node_t *
bfdev_rb_cached_delete_augmented(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *node,
                                 const struct bfdev_rb_callbacks *callbacks)
{
    bfdev_rb_node_t *leftmost;

    if (cached->leftmost != node)
        leftmost = NULL;
    else {
        leftmost = bfdev_rb_next(node);
        cached->leftmost = leftmost;
    }

    bfdev_rb_delete_augmented(&cached->root, node, callbacks);
    return leftmost;
}

/**
 * bfdev_rb_cached_replace - replace old cached node by new cached one.
 * @root: rbtree root of node.
 * @oldn: node to be replaced.
 * @newn: new node to insert.
 */
static inline void
bfdev_rb_cached_replace(bfdev_rb_root_cached_t *cached, bfdev_rb_node_t *oldn,
                        bfdev_rb_node_t *newn)
{
    if (cached->leftmost == oldn)
        cached->leftmost = newn;

    bfdev_rb_replace(&cached->root, oldn, newn);
}

#define BFDEV_RB_DECLARE_CALLBACKS(RBSTATIC, RBNAME, RBSTRUCT, RBFIELD, RBAUGMENTED, RBCOMPUTE) \
static void RBNAME##_rotate(bfdev_rb_node_t *rb_node, bfdev_rb_node_t *rb_successor)            \
{                                                                                               \
    RBSTRUCT *node = bfdev_rb_entry(rb_node, RBSTRUCT, RBFIELD);                                \
    RBSTRUCT *successor = bfdev_rb_entry(rb_successor, RBSTRUCT, RBFIELD);                      \
    successor->RBAUGMENTED = node->RBAUGMENTED;                                                 \
    RBCOMPUTE(node, false);                                                                     \
}                                                                                               \
                                                                                                \
static void RBNAME##_copy(bfdev_rb_node_t *rb_node, bfdev_rb_node_t *rb_successor)              \
{                                                                                               \
    RBSTRUCT *node = bfdev_rb_entry(rb_node, RBSTRUCT, RBFIELD);                                \
    RBSTRUCT *successor = bfdev_rb_entry(rb_successor, RBSTRUCT, RBFIELD);                      \
    successor->RBAUGMENTED = node->RBAUGMENTED;                                                 \
}                                                                                               \
                                                                                                \
static void RBNAME##_propagate(bfdev_rb_node_t *rb_node, bfdev_rb_node_t *rb_stop)              \
{                                                                                               \
    while (rb_node != rb_stop) {                                                                \
        RBSTRUCT *node = bfdev_rb_entry(rb_node, RBSTRUCT, RBFIELD);                            \
        if (RBCOMPUTE(node, true))                                                              \
            break;                                                                              \
        rb_node = node->RBFIELD.parent;                                                         \
    }                                                                                           \
}                                                                                               \
                                                                                                \
RBSTATIC struct bfdev_rb_callbacks RBNAME = {                                                   \
    .rotate = RBNAME##_rotate,                                                                  \
    .copy = RBNAME##_copy,                                                                      \
    .propagate = RBNAME##_propagate,                                                            \
}

#define BFDEV_RB_DECLARE_CALLBACKS_MAX(RBSTATIC, RBNAME, RBSTRUCT, RBFIELD, RBTYPE, RBAUGMENTED, RBCOMPUTE) \
static inline bool RBNAME##_compute_max(RBSTRUCT *node, bool exit)                                          \
{                                                                                                           \
    RBSTRUCT *child;                                                                                        \
    RBTYPE max = RBCOMPUTE(node);                                                                           \
    if (node->RBFIELD.left) {                                                                               \
        child = bfdev_rb_entry(node->RBFIELD.left, RBSTRUCT, RBFIELD);                                      \
        if (child->RBAUGMENTED > max)                                                                       \
            max = child->RBAUGMENTED;                                                                       \
    }                                                                                                       \
    if (node->RBFIELD.right) {                                                                              \
        child = bfdev_rb_entry(node->RBFIELD.right, RBSTRUCT, RBFIELD);                                     \
        if (child->RBAUGMENTED > max)                                                                       \
            max = child->RBAUGMENTED;                                                                       \
    }                                                                                                       \
    if (exit && node->RBAUGMENTED == max)                                                                   \
        return true;                                                                                        \
    node->RBAUGMENTED = max;                                                                                \
    return false;                                                                                           \
}                                                                                                           \
BFDEV_RB_DECLARE_CALLBACKS(RBSTATIC, RBNAME, RBSTRUCT, RBFIELD, RBAUGMENTED, RBNAME##_compute_max)

BFDEV_END_DECLS

#endif /* _BFDEV_RBTREE_H_ */
