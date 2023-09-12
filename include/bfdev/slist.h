/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SLIST_H_
#define _BFDEV_SLIST_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/stdbool.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

struct bfdev_slist_head {
    struct bfdev_slist_head *next;
};

#define BFDEV_SLIST_HEAD_STATIC \
    {NULL}

#define BFDEV_SLIST_HEAD_INIT \
    (struct bfdev_slist_head) BFDEV_SLIST_HEAD_STATIC

#define BFDEV_SLIST_HEAD(name) \
    struct bfdev_slist_head name = BFDEV_SLIST_HEAD_INIT

#ifdef BFDEV_DEBUG_SLIST
extern bool bfdev_slist_check_add(struct bfdev_slist_head *node, struct bfdev_slist_head *new);
extern bool bfdev_slist_check_del(struct bfdev_slist_head *node);
#endif

/**
 * bfdev_slist_head_init - initialize a slist head structure.
 * @head: slist head structure to be initialized.
 */
static inline void
bfdev_slist_head_init(struct bfdev_slist_head *head)
{
    head->next = NULL;
}

/**
 * bfdev_slist_add - add a new node next old node.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void
bfdev_slist_add(struct bfdev_slist_head *node, struct bfdev_slist_head *new)
{
#ifdef BFDEV_DEBUG_SLIST
    if (bfdev_unlikely(!bfdev_slist_check_add(node, new)))
        return;
#endif

    new->next = node->next;
    node->next = new;
}

/**
 * bfdev_slist_del - delete a node from slist.
 * @head: the head of the slist.
 * @entry: the element to delete from the slist.
 */
static inline void
bfdev_slist_del(struct bfdev_slist_head *head, struct bfdev_slist_head *node)
{
    struct bfdev_slist_head *walk = head;

#ifdef BFDEV_DEBUG_SLIST
    if (bfdev_unlikely(!bfdev_slist_check_del(node)))
        return;
#endif

    while (walk->next != node)
        walk = walk->next;

    walk->next = node->next;
    node->next = BFDEV_POISON_SLIST;
}

/**
 * bfdev_slist_check_empty - check whether the node is head.
 * @head: slist head to check.
 */
static inline bool
bfdev_slist_check_empty(const struct bfdev_slist_head *head)
{
    return !head->next;
}

/**
 * bfdev_slist_check_first - check whether the node is a header.
 * @head: the head of the slist.
 * @node: the entry to test.
 */
static inline bool
bfdev_slist_check_first(const struct bfdev_slist_head *head,
                        const struct bfdev_slist_head *node)
{
    return head->next == node;
}

/**
 * bfdev_slist_check_next - check whether the node is a ending.
 * @node: the node to check.
 */
static inline bool
bfdev_slist_check_end(const struct bfdev_slist_head *node)
{
    return !node->next;
}

/**
 * bfdev_slist_check_another - check whether has another node.
 * @head: slist head to check.
 * @node: the unique node.
 */
static inline bool
bfdev_slist_check_another(const struct bfdev_slist_head *head,
                          const struct bfdev_slist_head *node)
{
    return head->next == node && node->next == NULL;
}

/**
 * bfdev_slist_replace - replace a linked slist node with an external node.
 * @head: the head of the slist.
 * @old: the element to be replaced.
 * @new: the new element to insert.
 */
static inline void
bfdev_slist_replace(struct bfdev_slist_head *head, struct bfdev_slist_head *old,
                    struct bfdev_slist_head *new)
{
    struct bfdev_slist_head *walk = head;

    while (walk->next != old)
        walk = walk->next;

    walk->next = new;
    new->next = old->next;
}

/**
 * bfdev_slist_entry - get the struct for this entry.
 * @ptr: the struct slist head pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 */
#define bfdev_slist_entry(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

/**
 * bfdev_slist_first_entry - get the first element from a slist.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member:	the name of the slist head within the struct.
 */
#define bfdev_slist_first_entry(ptr, type, member) \
    bfdev_slist_entry((ptr)->next, type, member)

/**
 * bfdev_slist_next_entry - get the next element in slist.
 * @pos: the type * to cursor.
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_next_entry(pos, member) \
    bfdev_slist_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * bfdev_slist_for_each - iterate over a slist.
 * @pos: the struct slist head to use as a loop cursor.
 * @head: the head for your slist.
 */
#define bfdev_slist_for_each(pos, head) \
    for ((pos) = (head)->next; (pos); (pos) = (pos)->next)

/**
 * bfdev_slist_for_each_from - iterate over a slist from the current point.
 * @pos: the &struct slist head to use as a loop cursor.
 */
#define bfdev_slist_for_each_from(pos) \
    for (; (pos); (pos) = (pos)->next)

/**
 * bfdev_slist_for_each_continue - continue iteration over a slist.
 * @pos: the &struct slist head to use as a loop cursor.
 */
#define bfdev_slist_for_each_continue(pos) \
    for ((pos) = (pos)->next; (pos); (pos) = (pos)->next)

/**
 * bfdev_slist_for_each_safe - iterate over a slist safe against removal of slist entry.
 * @pos: the &struct slist head to use as a loop cursor.
 * @tmp: another slist head to use as temporary storage.
 * @head: the head for your slist.
 */
#define bfdev_slist_for_each_safe(pos, tmp, head) \
    for ((pos) = (head)->next, (tmp) = (pos)->next; \
         (pos); (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_slist_for_each_from_safe - iterate over a slist safe against removal of slist entry from the current point.
 * @pos: the &struct slist head to use as a loop cursor.
 * @tmp: another slist head to use as temporary storage.
 */
#define bfdev_slist_for_each_from_safe(pos, tmp) \
    for ((tmp) = (pos)->next; \
         (pos); (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_slist_for_each_continue_safe - continue slist iteration safe against removal.
 * @pos: the &struct slist head to use as a loop cursor.
 * @tmp: another slist head to use as temporary storage.
 */
#define bfdev_slist_for_each_continue_safe(pos, tmp) \
    for ((pos) = (pos)->next, (tmp) = (pos)->next; \
         (pos); (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_slist_for_each_entry - iterate over slist of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your slist.
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry(pos, head, member) \
    for ((pos) = bfdev_slist_first_entry(head, typeof(*(pos)), member); \
         (pos); (pos) = bfdev_slist_next_entry(pos, member))

/**
 * bfdev_slist_for_each_entry_from - iterate over slist of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_from(pos, member) \
    for (; (pos); (pos) = bfdev_slist_next_entry(pos, member))

/**
 * bfdev_slist_for_each_entry_continue - continue iteration over slist of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_continue(pos, member) \
    for ((pos) = bfdev_slist_next_entry(pos, member); \
         (pos); (pos) = bfdev_slist_next_entry(pos, member))

/**
 * bfdev_slist_for_each_entry_safe - iterate over slist of given type safe against removal of slist entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your slist.
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_slist_first_entry(head, typeof(*(pos)), member), \
         (tmp) = bfdev_slist_next_entry(pos, member); (pos); (pos) = (tmp), \
         ((tmp) && ((tmp) = bfdev_slist_next_entry(tmp, member))))

/**
 * bfdev_slist_for_each_entry_from_safe - iterate over slist from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member:	the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_from_safe(pos, tmp, member) \
    for ((tmp) = bfdev_slist_next_entry(pos, member); (pos); (pos) = (tmp), \
         ((tmp) && ((tmp) = bfdev_slist_next_entry(pos, member))))

/**
 * bfdev_slist_for_each_entry_continue_safe - continue slist iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member:	the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_continue_safe(pos, tmp, member) \
    for ((pos) = bfdev_slist_next_entry(pos, member), \
         (tmp) = bfdev_slist_next_entry(pos, member); (pos); (pos) = (tmp), \
         ((tmp) && ((tmp) = bfdev_slist_next_entry(pos, member))))

BFDEV_END_DECLS

#endif  /* _BFDEV_SLIST_H_ */
