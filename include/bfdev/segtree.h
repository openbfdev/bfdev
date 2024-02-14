/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SEGTREE_
#define _BFDEV_SEGTREE_

#include <bfdev/config.h>
#include <bfdev/rbtree.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_segtree_node bfdev_segtree_node_t;

struct bfdev_segtree_node {
    bfdev_rb_node_t node;
    unsigned long start, end;
    unsigned long subtree;
};

/**
 * bfdev_segtree_entry - get the struct for this entry.
 * @ptr: the &bfdev_segtree_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_segtree_node within the struct.
 */
#define bfdev_segtree_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_segtree_entry_safe - get the struct for this entry or null.
 * @ptr: the &bfdev_segtree_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_segtree_node within the struct.
 */
#define bfdev_segtree_entry_safe(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

extern void bfdev_segtree_insert(bfdev_rb_root_cached_t *root, bfdev_segtree_node_t *node);
extern void bfdev_segtree_delete(bfdev_rb_root_cached_t *root, bfdev_segtree_node_t *node);
extern bfdev_segtree_node_t *bfdev_segtree_search(bfdev_segtree_node_t *node, unsigned long start, unsigned long end);
extern bfdev_segtree_node_t *bfdev_segtree_first(bfdev_rb_root_cached_t *root, unsigned long start, unsigned long end);
extern bfdev_segtree_node_t *bfdev_segtree_next(bfdev_segtree_node_t *node, unsigned long start, unsigned long end);

/**
 * bfdev_segtree_first_entry - get the first element from a segtree.
 * @ptr: the rbtree root to take the element from.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_segtree_first_entry(root, start, end, type, member) \
    bfdev_segtree_entry_safe(bfdev_segtree_first(root, start, end), type, member)

/**
 * bfdev_segtree_next_entry - get the next element in segtree.
 * @pos: the type * to cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_segtree_next_entry(pos, start, end, member) \
    bfdev_segtree_entry_safe(bfdev_segtree_next(&(pos)->member, start, end), typeof(*(pos)), member)

/**
 * bfdev_segtree_for_each - iterate over a segtree.
 * @pos: the &bfdev_segtree_node_t to use as a loop cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 * @root: the root for your segtree.
 */
#define bfdev_segtree_for_each(pos, start, end, root) \
    for (pos = bfdev_segtree_first(root, start, end); pos; \
         pos = bfdev_segtree_next(pos, start, end))

/**
 * bfdev_segtree_for_each_form - iterate over a segtree from the current point.
 * @pos: the &bfdev_rb_node_t to use as a loop cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 */
#define bfdev_segtree_for_each_form(pos, start, end) \
    for (; pos; pos = bfdev_segtree_next(pos, start, end))

/**
 * bfdev_segtree_for_each_continue - continue iteration over a segtree.
 * @pos: the &bfdev_segtree_node_t to use as a loop cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 */
#define bfdev_segtree_for_each_continue(pos, start, end) \
    for ((void)(pos && (pos = bfdev_segtree_next(pos, start, end))); \
         pos; pos = bfdev_segtree_next(pos, start, end))

/**
 * bfdev_segtree_for_each_entry - iterate over segtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 * @root: the root for your segtree.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_segtree_for_each_entry(pos, start, end, root, member) \
    for (pos = bfdev_segtree_first_entry(root, start, end, typeof(*(pos)), member); \
         pos; pos = bfdev_segtree_next_entry(pos, start, end, member))

/**
 * bfdev_segtree_for_each_entry_from - iterate over segtree of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_segtree_for_each_entry_from(pos, start, end, member) \
    for (; pos; pos = bfdev_segtree_next_entry(pos, start, end, member))

/**
 * bfdev_segtree_for_each_entry_continue - continue iteration over segtree of given type.
 * @pos: the type * to use as a loop cursor.
 * @start: start endpoint of segtree element.
 * @end: end endpoint of segtree element.
 * @member: the name of the bfdev_rb_node within the struct.
 */
#define bfdev_segtree_for_each_entry_continue(pos, start, end, member) \
    for ((void)(pos && (pos = bfdev_segtree_next_entry(pos, start, end, member))); \
         pos; pos = bfdev_segtree_next_entry(pos, start, end, member))

#define BFDEV_SEGTREE_DEFINE(STSTATIC, STNAME, STSTRUCT, STRB, STTYPE, STSUBTREE, STSTART, STEND)       \
BFDEV_RB_DECLARE_CALLBACKS_MAX(static, STNAME##_callbacks, STSTRUCT, STRB, STTYPE, STSUBTREE, STEND);   \
STSTATIC void STNAME##_insert(bfdev_rb_root_cached_t *cached, STSTRUCT *node)                           \
{                                                                                                       \
    bfdev_rb_node_t **link = &cached->root.node;                                                        \
    STSTRUCT *parent = NULL;                                                                            \
    STTYPE start, end;                                                                                  \
    bool leftmost = true;                                                                               \
                                                                                                        \
    start = STSTART(node);                                                                              \
    end = STEND(node);                                                                                  \
                                                                                                        \
    while (*link) {                                                                                     \
        parent = bfdev_rb_entry(*link, STSTRUCT, STRB);                                                 \
        if (parent->STSUBTREE < end)                                                                    \
            parent->STSUBTREE = end;                                                                    \
        if (start < STSTART(parent))                                                                    \
            link = &parent->STRB.left;                                                                  \
        else {                                                                                          \
            link = &parent->STRB.right;                                                                 \
            leftmost = false;                                                                           \
        }                                                                                               \
    }                                                                                                   \
                                                                                                        \
    bfdev_rb_cached_insert_node_augmented(cached, parent ? &parent->STRB : NULL,                        \
                                          link, &node->STRB, leftmost, &STNAME##_callbacks);            \
    node->STSUBTREE = end;                                                                              \
}                                                                                                       \
                                                                                                        \
STSTATIC void STNAME##_delete(bfdev_rb_root_cached_t *cached, STSTRUCT *node)                           \
{                                                                                                       \
    bfdev_rb_cached_delete_augmented(cached, &node->STRB, &STNAME##_callbacks);                         \
}                                                                                                       \
                                                                                                        \
STSTATIC STSTRUCT *STNAME##_search(STSTRUCT *node, STTYPE start, STTYPE end)                            \
{                                                                                                       \
    STSTRUCT *walk;                                                                                     \
                                                                                                        \
    for (;;) {                                                                                          \
        if (node->STRB.left) {                                                                          \
            walk = bfdev_rb_entry(node->STRB.left, STSTRUCT, STRB);                                     \
            if (walk->STSUBTREE >= start) {                                                             \
                node = walk;                                                                            \
                continue;                                                                               \
            }                                                                                           \
        }                                                                                               \
        if (STSTART(node) <= end) {                                                                     \
            if (start <= STEND(node))                                                                   \
                return node;                                                                            \
            else if (node->STRB.right) {                                                                \
                node = bfdev_rb_entry(node->STRB.right, STSTRUCT, STRB);                                \
                if (node->STSUBTREE >= start)                                                           \
                    continue;                                                                           \
            }                                                                                           \
        }                                                                                               \
        return NULL;                                                                                    \
    }                                                                                                   \
}                                                                                                       \
                                                                                                        \
STSTATIC STSTRUCT *STNAME##_first(bfdev_rb_root_cached_t *cached, STTYPE start, STTYPE end)             \
{                                                                                                       \
    STSTRUCT *node, *leftmost;                                                                          \
                                                                                                        \
    if (!(node = bfdev_rb_entry_safe(cached->root.node, STSTRUCT, STRB)))                               \
        return NULL;                                                                                    \
                                                                                                        \
    if (node->STSUBTREE < start)                                                                        \
        return NULL;                                                                                    \
                                                                                                        \
    leftmost = bfdev_rb_cached_first_entry(cached, STSTRUCT, STRB);                                     \
    if (STSTART(leftmost) > end)                                                                        \
        return NULL;                                                                                    \
                                                                                                        \
    return STNAME##_search(node, start, end);                                                           \
}                                                                                                       \
                                                                                                        \
STSTATIC STSTRUCT *STNAME##_next(STSTRUCT *node, STTYPE start, STTYPE end)                              \
{                                                                                                       \
    bfdev_rb_node_t *prev, *walk = node->STRB.right;                                                    \
    STSTRUCT *right;                                                                                    \
                                                                                                        \
    for (;;) {                                                                                          \
        if ((right = bfdev_rb_entry_safe(walk, STSTRUCT, STRB)) && right->STSUBTREE >= start)           \
            return STNAME##_search(right, start, end);                                                  \
                                                                                                        \
        do {                                                                                            \
            walk = node->STRB.parent;                                                                   \
            if (!walk)                                                                                  \
                return NULL;                                                                            \
            prev = &node->STRB;                                                                         \
            node = bfdev_rb_entry(walk, STSTRUCT, STRB);                                                \
            walk = node->STRB.right;                                                                    \
        } while (walk == prev);                                                                         \
                                                                                                        \
        if (end < STSTART(node))                                                                        \
            return NULL;                                                                                \
        else if (start <= STEND(node))                                                                  \
            return node;                                                                                \
    }                                                                                                   \
}

BFDEV_END_DECLS

#endif /* _BFDEV_SEGTREE_ */
