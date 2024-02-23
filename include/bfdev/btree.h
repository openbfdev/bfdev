/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BTREE_H_
#define _BFDEV_BTREE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

struct bfdev_btree_layout {
    unsigned int keylen;
    unsigned int keynum;
    unsigned int ptrindex;
    size_t nodesize;
};

struct bfdev_btree_node {
    uintptr_t block[0];
};

struct bfdev_btree_root {
    const bfdev_alloc_t *alloc;
    struct bfdev_btree_layout *layout;
    struct bfdev_btree_node *node;
    unsigned int height;

    const struct bfdev_btree_ops *ops;
    void *pdata;
};

struct bfdev_btree_ops {
    void *(*alloc)(struct bfdev_btree_root *root);
    void (*free)(struct bfdev_btree_root *root, void *block);
    long (*find)(struct bfdev_btree_root *root, uintptr_t *node, uintptr_t *key);
    int (*clash)(struct bfdev_btree_root *root, void *clash, void *value);
    void *(*remove)(struct bfdev_btree_root *root, void *value);
};

#define BFDEV_BTREE_STATIC(LAYOUT, OPS, PDATA) \
    {.layout = (LAYOUT), .ops = (OPS), .pdata = (PDATA)}

#define BFDEV_BTREE_INIT(layout, ops, pdata) \
    (struct bfdev_btree_root) BFDEV_BTREE_STATIC(layout, ops, pdata)

#define BFDEV_BTREE_ROOT(name, layout, ops, pdata) \
    struct bfdev_btree_root name = BFDEV_BTREE_INIT(layout, ops, pdata)

extern struct bfdev_btree_layout
bfdev_btree_layout32;

extern struct bfdev_btree_layout
bfdev_btree_layout64;

extern struct bfdev_btree_layout
bfdev_btree_layoutptr;

static inline void
bfdev_btree_init(struct bfdev_btree_root *root,
                 struct bfdev_btree_layout *layout,
                 struct bfdev_btree_ops *ops, void *pdata)
{
    *root = BFDEV_BTREE_INIT(layout, ops, pdata);
}

extern long
bfdev_btree_key_find(struct bfdev_btree_root *root,
                     uintptr_t *node, uintptr_t *key);

extern void *
bfdev_btree_alloc(struct bfdev_btree_root *root);

extern void
bfdev_btree_free(struct bfdev_btree_root *root, void *node);

extern void *
bfdev_btree_lookup(struct bfdev_btree_root *root, uintptr_t *key);

extern int
bfdev_btree_update(struct bfdev_btree_root *root,
                   uintptr_t *key, void *value);

extern int
bfdev_btree_insert(struct bfdev_btree_root *root,
                   uintptr_t *key, void *value);

extern void *
bfdev_btree_remove(struct bfdev_btree_root *root,
                   uintptr_t *key);

extern void
bfdev_btree_destroy(struct bfdev_btree_root *root);

extern void
bfdev_btree_key_copy(struct bfdev_btree_root *root,
                     uintptr_t *dest, uintptr_t *src);

extern void *
bfdev_btree_first(struct bfdev_btree_root *root,
                  uintptr_t *key);

extern void *
bfdev_btree_last(struct bfdev_btree_root *root,
                 uintptr_t *key);

extern void *
bfdev_btree_next(struct bfdev_btree_root *root,
                 uintptr_t *key);

extern void *
bfdev_btree_prev(struct bfdev_btree_root *root,
                 uintptr_t *key);

/**
 * bfdev_btree_for_each - iterate over a btree.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 */
#define bfdev_btree_for_each(root, key, value)  \
    for (value = bfdev_btree_first(root, key);  \
         value; value = bfdev_btree_next(root, key))

/**
 * bfdev_btree_for_each_reverse - iterate over a btree backwards.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 */
#define bfdev_btree_for_each_reverse(root, key, value)  \
    for (value = bfdev_btree_last(root, key);           \
         value; value = bfdev_btree_prev(root, key))

/**
 * bfdev_btree_for_each_from - iterate over a btree from the current point.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 */
#define bfdev_btree_for_each_from(root, key, value) \
    for (; value; value = bfdev_btree_next(root, key))

/**
 * bfdev_btree_for_each_reverse_from - iterate over a btree backwards from the current point.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 */
#define bfdev_btree_for_each_reverse_from(root, key, value) \
    for (; value; value = bfdev_btree_prev(root, key))

/**
 * bfdev_btree_for_each_continue - continue iteration over a btree.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 */
#define bfdev_btree_for_each_continue(root, key, value) \
    for (value = bfdev_btree_next(root, key);           \
         value; value = bfdev_btree_next(root, key))

/**
 * bfdev_btree_for_each_reverse_continue - continue iteration over a btree backwards.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 */
#define bfdev_btree_for_each_reverse_continue(root, key, value) \
    for (value = bfdev_btree_prev(root, key);                   \
         value; value = bfdev_btree_prev(root, key))

/**
 * bfdev_btree_for_each_safe - iterate over btree safe against removal.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 * @tkey: another &key to use as temporary storage.
 * @tval: another value to use as temporary storage.
 */
#define bfdev_btree_for_each_safe(root, key, value, tkey, tval) \
    for ((void)((value = bfdev_btree_first(root, key)) &&       \
         (bfdev_btree_key_copy(root, tkey, key),                \
          tval = bfdev_btree_next(root, tkey))); value;         \
         bfdev_btree_key_copy(root, key, tkey),                 \
         value = tval, tval = bfdev_btree_next(root, tkey))

/**
 * bfdev_btree_for_each_reverse_safe - iterate backwards over btree safe against removal.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 * @tkey: another &key to use as temporary storage.
 * @tval: another value to use as temporary storage.
 */
#define bfdev_btree_for_each_reverse_safe(root, key, value, tkey, tval) \
    for ((void)((value = bfdev_btree_last(root, key)) &&                \
         (bfdev_btree_key_copy(root, tkey, key),                        \
          tval = bfdev_btree_prev(root, tkey))); value;                 \
         bfdev_btree_key_copy(root, key, tkey),                         \
         value = tval, tval = bfdev_btree_prev(root, tkey))

/**
 * bfdev_btree_for_each_from_safe - iterate over btree from current point safe against removal.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 * @tkey: another &key to use as temporary storage.
 * @tval: another value to use as temporary storage.
 */
#define bfdev_btree_for_each_from_safe(root, key, value, tkey, tval)    \
    for (btree_key_copy(root, tkey, key),                               \
         tval = bfdev_btree_next(root, tkey); value;                    \
         bfdev_btree_key_copy(root, key, tkey),                         \
         value = tval, tval = bfdev_btree_next(root, tkey))

/**
 * bfdev_btree_for_each_reverse_from_safe - iterate backwards over btree from current point safe against removal.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 * @tkey: another &key to use as temporary storage.
 * @tval: another value to use as temporary storage.
 */
#define bfdev_btree_for_each_reverse_from_safe(root, key, value, tkey, tval)    \
    for (btree_key_copy(root, tkey, key),                                       \
         tval = bfdev_btree_prev(root, tkey); value;                            \
         bfdev_btree_key_copy(root, key, tkey),                                 \
         value = tval, tval = bfdev_btree_prev(root, tkey))

/**
 * bfdev_btree_for_each_continue_safe - continue btree iteration safe against removal.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 * @tkey: another &key to use as temporary storage.
 * @tval: another value to use as temporary storage.
 */
#define bfdev_btree_for_each_continue_safe(root, key, value, tkey, tval)    \
    for ((void)((value = bfdev_btree_next(root, key)) &&                    \
         (bfdev_btree_key_copy(root, tkey, key),                            \
          tval = bfdev_btree_next(root, tkey))); value;                     \
         bfdev_btree_key_copy(root, key, tkey),                             \
         value = tval, tval = bfdev_btree_next(root, tkey))

/**
 * bfdev_btree_for_each_reverse_continue_safe - continue backwards over btree iteration safe against removal.
 * @root: the root for your btree.
 * @key: the &key to use as a loop cursor.
 * @value: the value of current loop cursor.
 * @tkey: another &key to use as temporary storage.
 * @tval: another value to use as temporary storage.
 */
#define bfdev_btree_for_each_reverse_continue_safe(root, key, value, tkey, tval)    \
    for ((void)((value = bfdev_btree_prev(root, key)) &&                            \
         (bfdev_btree_key_copy(root, tkey, key),                                    \
          tval = bfdev_btree_prev(root, tkey))); value;                             \
         bfdev_btree_key_copy(root, key, tkey),                                     \
         value = tval, tval = bfdev_btree_prev(root, tkey))

BFDEV_END_DECLS

#endif /* _BFDEV_BTREE_H_ */
