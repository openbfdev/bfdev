/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HASHTBL_H_
#define _BFDEV_HASHTBL_H_

#include <bfdev/config.h>
#include <bfdev/macro.h>
#include <bfdev/log2.h>
#include <bfdev/hash.h>
#include <bfdev/hlist.h>

BFDEV_BEGIN_DECLS

#define BFDEV_HASHTBL_CHECK(size) \
    (((size < 2) || (size & (size - 1))) ? -1 : size)

#define BFDEV_DECLARE_HASHTBL(name, size) \
    bfdev_hlist_head_t name[BFDEV_HASHTBL_CHECK(size)]

#define BFDEV_DEFINE_HASHTBL(name, size)        \
    BFDEV_DECLARE_HASHTBL(name, size) = {       \
        [0 ... BFDEV_HASHTBL_CHECK(size) - 1]   \
        = BFDEV_HLIST_HEAD_STATIC()             \
    }

#define BFDEV_HASHTBL_BITS(size) bfdev_ilog2(size)
#define BFDEV_HASHTBL_MASK(size) BFDEV_BIT_LOW_MASK(BFDEV_HASHTBL_BITS(size))

static __bfdev_always_inline unsigned long
bfdev_hashtbl_index(unsigned long size, unsigned long key)
{
    unsigned int bits;

    bits = BFDEV_HASHTBL_BITS(size);

    return bfdev_hashl(key, bits);
}

/**
 * bfdev_hash_init - initialize a hash table.
 * @hashtable: hashtable to be initialized.
 */
static inline void
bfdev_hashtbl_init(bfdev_hlist_head_t *head, unsigned long size)
{
    unsigned long count;

    for (count = 0; count < size; ++count)
        bfdev_hlist_head_init(&head[count]);
}

/**
 * bfdev_hash_empty - check whether a hashtable is empty.
 * @hashtable: hashtable to check.
 */
static inline bool
bfdev_hashtbl_empty(bfdev_hlist_head_t *head, unsigned long size)
{
    unsigned long count;

    for (count = 0; count < size; ++count) {
        if (!bfdev_hlist_check_empty(&head[count]))
            return false;
    }

    return true;
}

/**
 * bfdev_hashtbl_add - add an object to a hashtable.
 * @hashtbl: hashtable to add to.
 * @node: the &bfdev_hlist_node_t of the object to be added.
 * @key: the value of the object to be added.
 */
static inline void
bfdev_hashtbl_add(bfdev_hlist_head_t *head, unsigned long size,
                  bfdev_hlist_node_t *node, unsigned long key)
{
    unsigned long index = bfdev_hashtbl_index(size, key);
    bfdev_hlist_head_add(&head[index], node);
}

/**
 * bfdev_hashtbl_del - remove an object from a hashtable.
 * @node: &struct hlist_node of the object to remove.
 */
static inline void
bfdev_hashtbl_del(bfdev_hlist_node_t *node)
{
    bfdev_hlist_del_init(node);
}

/**
 * bfdev_hashtbl_for_each - iterate over a hashtable index.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @index: index to for each.
 */
#define bfdev_hashtbl_for_each_idx(pos, head, size, index) \
    bfdev_hlist_for_each(pos, &head[index & BFDEV_HASHTBL_MASK(size)])

/**
 * bfdev_hashtbl_for_each_safe - iterate over a hashtable index safe against
 * removal of hlist entry.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @index: index to for each.
 */
#define bfdev_hashtbl_for_each_idx_safe(pos, tmp, head, size, index) \
    bfdev_hlist_for_each_safe(pos, tmp, &head[index & BFDEV_HASHTBL_MASK(size)])

/**
 * bfdev_hashtbl_for_each_entry - iterate over hashtable index of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @member: the name of the bfdev_hlist_node within the struct.
 * @index: index to for each.
 */
#define bfdev_hashtbl_for_each_idx_entry(pos, head, size, member, index) \
    bfdev_hlist_for_each_entry(pos, &head[index & BFDEV_HASHTBL_MASK(size)], member)

/**
 * bfdev_hashtbl_for_each_entry_safe - iterate over hashtable index of given
 * type safe against removal of hlist entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @member: the name of the bfdev_hlist_node within the struct.
 * @index: index to for each.
 */
#define bfdev_hashtbl_for_each_idx_entry_safe(pos, tmp, head, size, member, index) \
    bfdev_hlist_for_each_entry_safe(pos, tmp, \
        &head[index & BFDEV_HASHTBL_MASK(size)], member)

/**
 * bfdev_hashtbl_for_each - iterate over a hashtable.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @index: index temporary storage.
 */
#define bfdev_hashtbl_for_each(pos, head, size, index) \
    for ((index) = 0; (index) < (size); ++(index)) \
        bfdev_hashtbl_for_each_idx(pos, head, size, index)

/**
 * bfdev_hashtbl_for_each_safe - iterate over a hashtable safe against
 * removal of hlist entry.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @index: index temporary storage.
 */
#define bfdev_hashtbl_for_each_safe(pos, tmp, head, size, index) \
    for ((index) = 0; (index) < (size); ++(index)) \
        bfdev_hashtbl_for_each_idx_safe(pos, tmp, head, size, index)

/**
 * bfdev_hashtbl_for_each_entry - iterate over hashtable of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @member: the name of the bfdev_hlist_node within the struct.
 * @index: index temporary storage.
 */
#define bfdev_hashtbl_for_each_entry(pos, head, size, member, index) \
    for ((index) = 0; (index) < (size); ++(index)) \
        bfdev_hashtbl_for_each_idx_entry(pos, head, size, member, index)

/**
 * bfdev_hashtbl_for_each_entry_safe - iterate over hashtable of given type
 * safe against removal of hlist entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your hashtable.
 * @size: the size of your hashtable.
 * @member: the name of the bfdev_hlist_node within the struct.
 * @index: index temporary storage.
 */
#define bfdev_hashtbl_for_each_entry_safe(pos, tmp, head, size, member, index) \
    for ((index) = 0; (index) < (size); ++(index)) \
        bfdev_hashtbl_for_each_idx_entry_safe(pos, tmp, head, size, member, index)

BFDEV_END_DECLS

#endif /* _BFDEV_HASHTBL_H_ */
