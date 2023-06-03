/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/stddef.h>
#include <bfdev/limits.h>
#include <bfdev/stdbool.h>
#include <bfdev/compiler.h>
#include <bfdev/attributes.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

#define BFDEV_RB_RED    (0)
#define BFDEV_RB_BLACK  (1)
#define BFDEV_RB_NSET   (2)

struct bfdev_rb_node {
    struct bfdev_rb_node *parent;
    struct bfdev_rb_node *left;
    struct bfdev_rb_node *right;
    bool color;
};

struct bfdev_rb_root {
    struct bfdev_rb_node *node;
};

struct bfdev_rb_root_cached {
    struct bfdev_rb_root root;
    struct bfdev_rb_node *leftmost;
};

struct bfdev_rb_callbacks {
    void (*rotate)(struct bfdev_rb_node *node, struct bfdev_rb_node *successor);
    void (*copy)(struct bfdev_rb_node *node, struct bfdev_rb_node *successor);
    void (*propagate)(struct bfdev_rb_node *node, struct bfdev_rb_node *stop);
};

#define BFDEV_RB_STATIC \
    {NULL}

#define BFDEV_RB_CACHED_STATIC \
    {{NULL}, NULL}

#define BFDEV_RB_INIT \
    (struct bfdev_rb_root) BFDEV_RB_STATIC

#define BFDEV_RB_CACHED_INIT \
    (struct bfdev_rb_root_cached) BFDEV_RB_CACHED_STATIC

#define BFDEV_RB_ROOT(name) \
    struct bfdev_rb_root name = BFDEV_RB_INIT

#define BFDEV_RB_ROOT_CACHED(name) \
    struct bfdev_rb_root_cached name = BFDEV_RB_CACHED_INIT

#define BFDEV_RB_EMPTY_ROOT(root) \
    ((root)->node == NULL)

#define BFDEV_RB_EMPTY_ROOT_CACHED(cached) \
    ((cached)->root.node == NULL)

#define BFDEV_RB_EMPTY_NODE(node) \
    ((node)->parent == (node))

#define BFDEV_RB_CLEAR_NODE(node) \
    ((node)->parent = (node))

/**
 * bfdev_rb_entry - get the struct for this entry.
 * @ptr: the &struct bfdev_rb_node pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * bfdev_rb_entry_safe - get the struct for this entry or null.
 * @ptr: the &struct bfdev_rb_node pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_rb_entry_safe(ptr, type, member) \
    container_of_safe(ptr, type, member)

#ifdef BFDEV_DEBUG_RBTREE
extern bool bfdev_rb_debug_link_check(struct bfdev_rb_node *parent, struct bfdev_rb_node **link, struct bfdev_rb_node *node);
extern bool bfdev_rb_debug_delete_check(struct bfdev_rb_node *node);
#endif

typedef long (*bfdev_rb_find_t)(const struct bfdev_rb_node *node, const void *key);
typedef long (*bfdev_rb_cmp_t)(const struct bfdev_rb_node *nodea, const struct bfdev_rb_node *nodeb);

extern void bfdev_rb_fixup_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *node, const struct bfdev_rb_callbacks *callbacks);
extern void bfdev_rb_erase_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *parent, const struct bfdev_rb_callbacks *callbacks);
extern struct bfdev_rb_node *bfdev_rb_remove_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *node, const struct bfdev_rb_callbacks *callbacks);
extern void bfdev_rb_fixup(struct bfdev_rb_root *root, struct bfdev_rb_node *node);
extern void bfdev_rb_erase(struct bfdev_rb_root *root, struct bfdev_rb_node *parent);
extern struct bfdev_rb_node *bfdev_rb_remove(struct bfdev_rb_root *root, struct bfdev_rb_node *node);

extern void bfdev_rb_replace(struct bfdev_rb_root *root, struct bfdev_rb_node *old, struct bfdev_rb_node *new);
extern struct bfdev_rb_node *bfdev_rb_find(const struct bfdev_rb_root *root, const void *key, bfdev_rb_find_t cmp);
extern struct bfdev_rb_node *bfdev_rb_find_last(struct bfdev_rb_root *root, const void *key, bfdev_rb_find_t cmp, struct bfdev_rb_node **parentp, struct bfdev_rb_node ***linkp);
extern struct bfdev_rb_node **bfdev_rb_parent(struct bfdev_rb_root *root, struct bfdev_rb_node **parentp, struct bfdev_rb_node *node, bfdev_rb_cmp_t cmp, bool *leftmost);
extern struct bfdev_rb_node **bfdev_rb_parent_conflict(struct bfdev_rb_root *root, struct bfdev_rb_node **parentp, struct bfdev_rb_node *node, bfdev_rb_cmp_t cmp, bool *leftmost);

#define bfdev_rb_cached_erase_augmented(cached, parent, callbacks) bfdev_rb_erase_augmented(&(cached)->root, parent, callbacks)
#define bfdev_rb_cached_remove_augmentebfdev_rb_pre_nextd(cached, node, callbacks) bfdev_rb_remove_augmented(&(cached)->root, node, callbacks)
#define bfdev_rb_cached_erase(cached, parent) bfdev_rb_erase(&(cached)->root, parent)
#define bfdev_rb_cached_remove(cached, node) bfdev_rb_remove(&(cached)->root, node)
#define bfdev_rb_cached_find(cached, key, cmp) bfdev_rb_find(&(cached)->root, key, cmp)
#define bfdev_rb_cached_find_last(cached, key, cmp, parentp, linkp) bfdev_rb_find_last(&(cached)->root, key, cmp, parentp, linkp)
#define bfdev_rb_cached_parent(cached, parentp, node, cmp, leftmost) bfdev_rb_parent(&(cached)->root, parentp, node, cmp, leftmost)
#define bfdev_rb_cached_parent_conflict(cached, parentp, node, cmp, leftmost) bfdev_rb_parent_conflict(&(cached)->root, parentp, node, cmp, leftmost)

extern struct bfdev_rb_node *bfdev_rb_left_far(const struct bfdev_rb_node *node);
extern struct bfdev_rb_node *bfdev_rb_right_far(const struct bfdev_rb_node *node);
extern struct bfdev_rb_node *bfdev_rb_left_deep(const struct bfdev_rb_node *node);
extern struct bfdev_rb_node *bfdev_rb_right_deep(const struct bfdev_rb_node *node);

/* Inorder iteration (Sequential) - find logical next and previous nodes */
extern struct bfdev_rb_node *bfdev_rb_first(const struct bfdev_rb_root *root);
extern struct bfdev_rb_node *bfdev_rb_last(const struct bfdev_rb_root *root);
extern struct bfdev_rb_node *bfdev_rb_prev(const struct bfdev_rb_node *node);
extern struct bfdev_rb_node *bfdev_rb_next(const struct bfdev_rb_node *node);

/* Preorder iteration (Root-first) - always access the left node first */
extern struct bfdev_rb_node *bfdev_rb_pre_first(const struct bfdev_rb_root *root);
extern struct bfdev_rb_node *bfdev_rb_pre_next(const struct bfdev_rb_node *node);

/* Postorder iteration (Depth-first) - always visit the parent after its children */
extern struct bfdev_rb_node *bfdev_rb_post_first(const struct bfdev_rb_root *root);
extern struct bfdev_rb_node *bfdev_rb_post_next(const struct bfdev_rb_node *node);

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
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_for_each(pos, root) \
    for (pos = bfdev_rb_first(root); pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_for_each_reverse - iterate over a rbtree backwards.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_for_each_reverse(pos, root) \
    for (pos = bfdev_rb_last(root); pos; pos = bfdev_rb_prev(pos))

/**
 * bfdev_rb_for_each_from - iterate over a rbtree from the current point.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 */
#define bfdev_rb_for_each_from(pos) \
    for (; pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_for_each_reverse_from - iterate over a rbtree backwards from the current point.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 */
#define bfdev_rb_for_each_reverse_from(pos) \
    for (; pos; pos = bfdev_rb_prev(pos))

/**
 * bfdev_rb_for_each_continue - continue iteration over a rbtree.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 */
#define bfdev_rb_for_each_continue(pos) \
    for (pos = bfdev_rb_next(pos); pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_for_each_reverse_continue - continue iteration over a rbtree backwards.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
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
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_pre_for_each(pos, root) \
    for (pos = bfdev_rb_pre_first(root); pos; pos = bfdev_rb_pre_next(pos))

/**
 * bfdev_rb_pre_for_each_from - preorder iterate over a rbtree from the current point.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 */
#define bfdev_rb_pre_for_each_from(pos) \
    for (; pos; pos = bfdev_rb_pre_next(pos))

/**
 * bfdev_rb_pre_for_each_continue - continue preorder iteration over a rbtree.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
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
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_post_for_each(pos, root) \
    for (pos = bfdev_rb_post_first(root); pos; pos = bfdev_rb_post_next(pos))

/**
 * bfdev_rb_post_for_each_from - postorder iterate over a rbtree from the current point.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 */
#define bfdev_rb_post_for_each_from(pos) \
    for (; pos; pos = bfdev_rb_post_next(pos))

/**
 * bfdev_rb_post_for_each_continue - continue postorder iteration over a rbtree.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 */
#define bfdev_rb_post_for_each_continue(pos) \
    for (pos = bfdev_rb_post_next(pos); pos; pos = bfdev_rb_post_next(pos))

/**
 * bfdev_rb_post_for_each_safe - postorder iterate over a rbtree safe against removal of rbtree entry.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @tmp: another bfdev_rb_node to use as temporary storage.
 * @root: the root for your rbtree.
 */
#define bfdev_rb_post_for_each_safe(pos, tmp, root) \
    for (pos = bfdev_rb_post_first(root); pos && \
        ({tmp = bfdev_rb_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_safe_from - postorder iterate over a rbtree safe against removal of rbtree entry from the current point.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @tmp: another bfdev_rb_node to use as temporary storage.
 */
#define bfdev_rb_post_for_each_safe_from(pos, tmp) \
    for (; pos && ({tmp = bfdev_rb_post_next(pos); 1; }); pos = tmp)

/**
 * bfdev_rb_post_for_each_safe_continue - continue rbtree postorder iteration safe against removal.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
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
bfdev_rb_link(struct bfdev_rb_node *parent, struct bfdev_rb_node **link, struct bfdev_rb_node *node)
{
#ifdef BFDEV_DEBUG_RBTREE
    if (unlikely(!rb_debug_link_check(parent, link, node)))
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
bfdev_rb_insert_node(struct bfdev_rb_root *root, struct bfdev_rb_node *parent,
                     struct bfdev_rb_node **link, struct bfdev_rb_node *node)
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
bfdev_rb_insert(struct bfdev_rb_root *root, struct bfdev_rb_node *node, bfdev_rb_cmp_t cmp)
{
    struct bfdev_rb_node *parent, **link;

    link = bfdev_rb_parent(root, &parent, node, cmp, NULL);
    bfdev_rb_insert_node(root, parent, link, node);
}

/**
 * bfdev_rb_insert_conflict - find the parent node and insert new node or conflict.
 * @root: rbtree root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline bool
bfdev_rb_insert_conflict(struct bfdev_rb_root *root, struct bfdev_rb_node *node, bfdev_rb_cmp_t cmp)
{
    struct bfdev_rb_node *parent, **link;

    link = bfdev_rb_parent_conflict(root, &parent, node, cmp, NULL);
    if (!link)
        return true;

    bfdev_rb_insert_node(root, parent, link, node);
    return false;
}

/**
 * bfdev_rb_delete - delete node and fixup rbtree.
 * @root: rbtree root of node.
 * @node: node to delete.
 */
static inline void
bfdev_rb_delete(struct bfdev_rb_root *root, struct bfdev_rb_node *node)
{
    struct bfdev_rb_node *rebalance;

#ifdef BFDEV_DEBUG_RBTREE
    if (unlikely(!rb_debug_delete_check(node)))
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
bfdev_rb_insert_node_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *parent,
                               struct bfdev_rb_node **link, struct bfdev_rb_node *node,
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
bfdev_rb_insert_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *node,
                          bfdev_rb_cmp_t cmp, const struct bfdev_rb_callbacks *callbacks)
{
    struct bfdev_rb_node *parent, **link;

    link = bfdev_rb_parent(root, &parent, node, cmp, NULL);
    bfdev_rb_insert_node_augmented(root, parent, link, node, callbacks);
}

/**
 * bfdev_rb_insert_conflict_augmented - augmented find the parent node and insert new node or conflict.
 * @root: rbtree root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 * @callbacks: augmented callback function.
 */
static inline bool
bfdev_rb_insert_conflict_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *node,
                                   bfdev_rb_cmp_t cmp, const struct bfdev_rb_callbacks *callbacks)
{
    struct bfdev_rb_node *parent, **link;

    link = bfdev_rb_parent_conflict(root, &parent, node, cmp, NULL);
    if (!link)
        return true;

    bfdev_rb_insert_node_augmented(root, parent, link, node, callbacks);
    return false;
}

/**
 * bfdev_rb_delete_augmented - augmented delete node and fixup rbtree.
 * @root: rbtree root of node.
 * @node: node to delete.
 * @callbacks: augmented callback function.
 */
static inline void
bfdev_rb_delete_augmented(struct bfdev_rb_root *root, struct bfdev_rb_node *node,
                          const struct bfdev_rb_callbacks *callbacks)
{
    struct bfdev_rb_node *rebalance;

#ifdef BFDEV_DEBUG_RBTREE
    if (unlikely(!rb_debug_delete_check(node)))
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
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_for_each(pos, cached) \
    for (pos = bfdev_rb_cached_first(cached); pos; pos = bfdev_rb_next(pos))

/**
 * bfdev_rb_cached_for_each_reverse - iterate over a cached rbtree backwards.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
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
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
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
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
 * @cached: the cached root for your rbtree.
 */
#define bfdev_rb_cached_post_for_each(pos, cached) \
    bfdev_rb_post_for_each(pos, &(cached)->root)

/**
 * bfdev_rb_cached_post_for_each_safe - postorder iterate over a cached rbtree safe against removal of rbtree entry.
 * @pos: the &struct bfdev_rb_node to use as a loop cursor.
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
bfdev_rb_cached_fixup(struct bfdev_rb_root_cached *cached,
                      struct bfdev_rb_node *node, bool leftmost)
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
bfdev_rb_cached_insert_node(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *parent,
                            struct bfdev_rb_node **link, struct bfdev_rb_node *node, bool leftmost)
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
bfdev_rb_cached_insert(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node,
                       bfdev_rb_cmp_t cmp)
{
    struct bfdev_rb_node *parent, **link;
    bool leftmost = true;

    link = bfdev_rb_cached_parent(cached, &parent, node, cmp, &leftmost);
    bfdev_rb_cached_insert_node(cached, parent, link, node, leftmost);
}

/**
 * bfdev_rb_cached_insert_conflict - find the parent node and insert new cached node or conflict.
 * @cached: rbtree cached root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline bool
bfdev_rb_cached_insert_conflict(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node,
                                bfdev_rb_cmp_t cmp)
{
    struct bfdev_rb_node *parent, **link;
    bool leftmost = true;

    link = bfdev_rb_cached_parent_conflict(cached, &parent, node, cmp, &leftmost);
    if (!link)
        return true;

    bfdev_rb_cached_insert_node(cached, parent, link, node, leftmost);
    return false;
}

/**
 * bfdev_rb_cached_delete - delete cached node and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @node: node to delete.
 */
static inline struct bfdev_rb_node *
bfdev_rb_cached_delete(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node)
{
    struct bfdev_rb_node *leftmost = NULL;

    if (cached->leftmost == node)
        leftmost = cached->leftmost = bfdev_rb_next(node);

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
bfdev_rb_cached_fixup_augmented(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node,
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
bfdev_rb_cached_insert_node_augmented(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *parent,
                                      struct bfdev_rb_node **link, struct bfdev_rb_node *node,
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
bfdev_rb_cached_insert_augmented(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node,
                                 bfdev_rb_cmp_t cmp, const struct bfdev_rb_callbacks *callbacks)
{
    struct bfdev_rb_node *parent, **link;
    bool leftmost = true;

    link = bfdev_rb_cached_parent(cached, &parent, node, cmp, &leftmost);
    bfdev_rb_cached_insert_node_augmented(cached, parent, link, node, leftmost, callbacks);
}

/**
 * bfdev_rb_cached_insert_conflict - find the parent node and insert new cached node or conflict.
 * @cached: rbtree cached root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 * @callbacks: augmented callback function.
 */
static inline bool
bfdev_rb_cached_insert_conflict_augmented(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node,
                                          bfdev_rb_cmp_t cmp, const struct bfdev_rb_callbacks *callbacks)
{
    struct bfdev_rb_node *parent, **link;
    bool leftmost = true;

    link = bfdev_rb_cached_parent_conflict(cached, &parent, node, cmp, &leftmost);
    if (!link)
        return true;

    bfdev_rb_cached_insert_node_augmented(cached, parent, link, node, leftmost, callbacks);
    return false;
}

/**
 * bfdev_rb_cached_delete - delete cached node and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @node: node to delete.
 * @callbacks: augmented callback function.
 */
static inline struct bfdev_rb_node *
bfdev_rb_cached_delete_augmented(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *node,
                                 const struct bfdev_rb_callbacks *callbacks)
{
    struct bfdev_rb_node *leftmost = NULL;

    if (cached->leftmost == node)
        leftmost = cached->leftmost = bfdev_rb_next(node);

    bfdev_rb_delete_augmented(&cached->root, node, callbacks);
    return leftmost;
}

/**
 * bfdev_rb_cached_replace - replace old cached node by new cached one.
 * @root: rbtree root of node.
 * @old: node to be replaced.
 * @new: new node to insert.
 */
static inline void
bfdev_rb_cached_replace(struct bfdev_rb_root_cached *cached, struct bfdev_rb_node *old,
                        struct bfdev_rb_node *new)
{
    if (cached->leftmost == old)
        cached->leftmost = new;

    bfdev_rb_replace(&cached->root, old, new);
}

#define BFDEV_RB_DECLARE_CALLBACKS(RBSTATIC, RBNAME, RBSTRUCT, RBFIELD, RBAUGMENTED, RBCOMPUTE)     \
static void RBNAME##_rotate(struct bfdev_rb_node *rb_node, struct bfdev_rb_node *rb_successor)      \
{                                                                                                   \
    RBSTRUCT *node = bfdev_rb_entry(rb_node, RBSTRUCT, RBFIELD);                                    \
    RBSTRUCT *successor = bfdev_rb_entry(rb_successor, RBSTRUCT, RBFIELD);                          \
    successor->RBAUGMENTED = node->RBAUGMENTED;                                                     \
    RBCOMPUTE(node, false);                                                                         \
}                                                                                                   \
                                                                                                    \
static void RBNAME##_copy(struct bfdev_rb_node *rb_node, struct bfdev_rb_node *rb_successor)        \
{                                                                                                   \
    RBSTRUCT *node = bfdev_rb_entry(rb_node, RBSTRUCT, RBFIELD);                                    \
    RBSTRUCT *successor = bfdev_rb_entry(rb_successor, RBSTRUCT, RBFIELD);                          \
    successor->RBAUGMENTED = node->RBAUGMENTED;                                                     \
}                                                                                                   \
                                                                                                    \
static void RBNAME##_propagate(struct bfdev_rb_node *rb_node, struct bfdev_rb_node *rb_stop)        \
{                                                                                                   \
    while (rb_node != rb_stop) {                                                                    \
        RBSTRUCT *node = bfdev_rb_entry(rb_node, RBSTRUCT, RBFIELD);                                \
        if (RBCOMPUTE(node, true))                                                                  \
            break;                                                                                  \
        rb_node = node->RBFIELD.parent;                                                             \
    }                                                                                               \
}                                                                                                   \
                                                                                                    \
RBSTATIC struct bfdev_rb_callbacks RBNAME = {                                                       \
    .rotate = RBNAME##_rotate,                                                                      \
    .copy = RBNAME##_copy,                                                                          \
    .propagate = RBNAME##_propagate,                                                                \
}

#define BFDEV_RB_DECLARE_CALLBACKS_MAX(RBSTATIC, RBNAME, RBSTRUCT, RBFIELD, RBTYPE, RBAUGMENTED, RBCOMPUTE)     \
static inline bool RBNAME##_compute_max(RBSTRUCT *node, bool exit)                                              \
{                                                                                                               \
    RBSTRUCT *child;                                                                                            \
    RBTYPE max = RBCOMPUTE(node);                                                                               \
    if (node->RBFIELD.left) {                                                                                   \
        child = bfdev_rb_entry(node->RBFIELD.left, RBSTRUCT, RBFIELD);                                          \
        if (child->RBAUGMENTED > max)                                                                           \
            max = child->RBAUGMENTED;                                                                           \
    }                                                                                                           \
    if (node->RBFIELD.right) {                                                                                  \
        child = bfdev_rb_entry(node->RBFIELD.right, RBSTRUCT, RBFIELD);                                         \
        if (child->RBAUGMENTED > max)                                                                           \
            max = child->RBAUGMENTED;                                                                           \
    }                                                                                                           \
    if (exit && node->RBAUGMENTED == max)                                                                       \
        return true;                                                                                            \
    node->RBAUGMENTED = max;                                                                                    \
    return false;                                                                                               \
}                                                                                                               \
BFDEV_RB_DECLARE_CALLBACKS(RBSTATIC, RBNAME, RBSTRUCT, RBFIELD, RBAUGMENTED, RBNAME##_compute_max)

BFDEV_END_DECLS

#endif /* _RBTREE_H_ */
