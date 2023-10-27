/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SKIPLIST_H_
#define _BFDEV_SKIPLIST_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/errno.h>
#include <bfdev/list.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

struct bfdev_skip_node {
    void *pdata;
    struct bfdev_list_head list[0];
};

struct bfdev_skip_head {
    const struct bfdev_alloc *alloc;
    unsigned int curr;
    unsigned int levels;
    struct bfdev_list_head nodes[0];
};

extern int
bfdev_skiplist_insert(struct bfdev_skip_head *head, void *data,
                      bfdev_cmp_t cmp, void *pdata);

extern void
bfdev_skiplist_delete(struct bfdev_skip_head *head,
                      bfdev_find_t find, void *pdata);

extern struct bfdev_skip_node *
bfdev_skiplist_find(struct bfdev_skip_head *head,
                    bfdev_find_t find, void *pdata);

extern void
bfdev_skiplist_reset(struct bfdev_skip_head *head,
                     bfdev_release_t relse);

extern void
bfdev_skiplist_destroy(struct bfdev_skip_head *head,
                       bfdev_release_t relse);

extern struct bfdev_skip_head *
bfdev_skiplist_create(const struct bfdev_alloc *alloc,
                      unsigned int levels);

/**
 * bfdev_skiplist_for_each - iterate over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each(pos, head, level) \
    bfdev_list_for_each_entry(pos, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_reverse - iterate backwards over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_reverse(pos, head, level) \
    bfdev_list_for_each_entry_reverse(pos, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_from - iterate over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_from(pos, head, level) \
    bfdev_list_for_each_entry_from(pos, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_reverse_from - iterate backwards over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_reverse_from(pos, head, level) \
    bfdev_list_for_each_entry_reverse_from(pos, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_continue - continue iteration over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_continue(pos, head, level) \
    bfdev_list_for_each_entry_continue(pos, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_reverse_continue - iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_reverse_continue(pos, head, level) \
    bfdev_list_for_each_entry_reverse_continue(pos, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_safe - iterate over list of given type safe against removal of list entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_safe(pos, tmp, head, level) \
    bfdev_list_for_each_entry_safe(pos, tmp, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_reverse_safe(pos, tmp, head, level) \
    bfdev_list_for_each_entry_reverse_safe(pos, tmp, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_from_safe - iterate over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_from_safe(pos, tmp, head, level) \
    bfdev_list_for_each_entry_from_safe(pos, tmp, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_reverse_from_safe - iterate backwards over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_reverse_from_safe(pos, tmp, head, level) \
    bfdev_list_for_each_entry_reverse_from_safe(pos, tmp, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_continue_safe - continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_continue_safe(pos, tmp, head, level) \
    bfdev_list_for_each_entry_continue_safe(pos, tmp, &(head)->nodes[level], list[level])

/**
 * bfdev_skiplist_for_each_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_skiplist_for_each_reverse_continue_safe(pos, tmp, head, level) \
    bfdev_list_for_each_entry_reverse_continue_safe(pos, tmp, &(head)->nodes[level], list[level])

BFDEV_END_DECLS

#endif /* _BFDEV_SKIPLIST_H_ */
