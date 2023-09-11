/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BTREE_H_
#define _BFDEV_BTREE_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>

BFDEV_BEGIN_DECLS

struct bfdev_btree_root;
typedef void *(*bfdev_btree_alloc_t)(struct bfdev_btree_root *root);
typedef void (*bfdev_btree_free_t)(struct bfdev_btree_root *root, void *block);
typedef long (*bfdev_btree_find_t)(struct bfdev_btree_root *root, uintptr_t *node, uintptr_t *key);
typedef int (*bfdev_btree_clash_t)(struct bfdev_btree_root *root, void *clash, void *value);
typedef void *(*bfdev_btree_remove_t)(struct bfdev_btree_root *root, void *value);

struct bfdev_btree_layout {
    unsigned int keylen;
    unsigned int keynum;
    unsigned int ptrindex;
    size_t nodesize;
};

struct bfdev_btree_root {
    struct bfdev_btree_layout *layout;
    struct bfdev_btree_node *node;
    unsigned int height;

    bfdev_btree_alloc_t alloc;
    bfdev_btree_free_t free;
    bfdev_btree_find_t find;
    bfdev_btree_clash_t clash;
    bfdev_btree_remove_t remove;
    void *pdata;
};

struct bfdev_btree_node {
    uintptr_t block[0];
};

#define BFDEV_BTREE_ROOT_STATIC(_layout, _alloc, _free, _find,  \
                                _clash, _remove, _pdata) {      \
    .layout = (_layout), .alloc = (_alloc), .free = (_free),    \
    .find = (_find), .clash = (_clash), .remove = (_remove),    \
    .pdata = (_pdata),                                          \
}

#define BFDEV_BTREE_ROOT_INIT(layout, alloc, free, find,    \
                        clash, remove, pdata)               \
    (struct bfdev_btree_root) BFDEV_BTREE_ROOT_STATIC       \
        (layout, alloc, free, find, clash, remove, pdata)

#define BFDEV_BTREE_ROOT(name, layout, alloc, free, find,   \
                         clash, remove, pdata)              \
    struct bfdev_btree_root name = BFDEV_BTREE_ROOT_INIT    \
        (layout, alloc, free, find, clash, remove, pdata)

extern struct bfdev_btree_layout bfdev_btree_layout32;
extern struct bfdev_btree_layout bfdev_btree_layout64;
extern struct bfdev_btree_layout bfdev_btree_layoutptr;

extern long bfdev_btree_key_find(struct bfdev_btree_root *root, uintptr_t *node, uintptr_t *key);
extern void *bfdev_btree_alloc(struct bfdev_btree_root *root);
extern void bfdev_btree_free(struct bfdev_btree_root *root, void *node);

extern void *bfdev_btree_lookup(struct bfdev_btree_root *root, uintptr_t *key);
extern int bfdev_btree_update(struct bfdev_btree_root *root, uintptr_t *key, void *value);
extern int bfdev_btree_insert(struct bfdev_btree_root *root, uintptr_t *key, void *value);
extern void *bfdev_btree_remove(struct bfdev_btree_root *root, uintptr_t *key);
extern void bfdev_btree_destroy(struct bfdev_btree_root *root);

extern void bfdev_btree_key_copy(struct bfdev_btree_root *root, uintptr_t *dest, uintptr_t *src);
extern void *bfdev_btree_first(struct bfdev_btree_root *root, uintptr_t *key);
extern void *bfdev_btree_last(struct bfdev_btree_root *root, uintptr_t *key);
extern void *bfdev_btree_next(struct bfdev_btree_root *root, uintptr_t *key);
extern void *bfdev_btree_prev(struct bfdev_btree_root *root, uintptr_t *key);

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
    for (value = bfdev_btree_first(root, key),                  \
         bfdev_btree_key_copy(root, tkey, key),                 \
         tval = bfdev_btree_next(root, tkey); value;            \
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
    for (value = bfdev_btree_last(root, key),                           \
         bfdev_btree_key_copy(root, tkey, key),                         \
         tval = bfdev_btree_prev(root, tkey); value;                    \
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
    for (value = bfdev_btree_next(root, key),                               \
         bfdev_btree_key_copy(root, tkey, key),                             \
         tval = bfdev_btree_next(root, tkey); value;                        \
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
    for (value = bfdev_btree_prev(root, key),                                       \
         bfdev_btree_key_copy(root, tkey, key),                                     \
         tval = bfdev_btree_prev(root, tkey); value;                                \
         bfdev_btree_key_copy(root, key, tkey),                                     \
         value = tval, tval = bfdev_btree_prev(root, tkey))

BFDEV_END_DECLS

#endif /* _BFDEV_BTREE_H_ */
