/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SLIST_H_
#define _BFDEV_SLIST_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_slist_head bfdev_slist_head_t;

struct bfdev_slist_head {
    bfdev_slist_head_t *next;
};

#define BFDEV_SLIST_HEAD_STATIC() { \
    .next = NULL, \
}

#define BFDEV_SLIST_HEAD_INIT() \
    (bfdev_slist_head_t) BFDEV_SLIST_HEAD_STATIC()

#define BFDEV_SLIST_HEAD(name) \
    bfdev_slist_head_t name = BFDEV_SLIST_HEAD_INIT()

#ifdef BFDEV_DEBUG_SLIST
extern bool
bfdev_slist_check_add(bfdev_slist_head_t *node, bfdev_slist_head_t *newn);

extern bool
bfdev_slist_check_del(bfdev_slist_head_t *node);
#endif

/**
 * bfdev_slist_head_init - initialize a slist head structure.
 * @head: slist head structure to be initialized.
 */
static inline void
bfdev_slist_head_init(bfdev_slist_head_t *head)
{
    *head = BFDEV_SLIST_HEAD_INIT();
}

/**
 * bfdev_slist_add - add a new node next old node.
 * @node: list head to add it next.
 * @newn: new entry to be added.
 */
static inline void
bfdev_slist_add(bfdev_slist_head_t *node, bfdev_slist_head_t *newn)
{
#ifdef BFDEV_DEBUG_SLIST
    if (bfdev_unlikely(!bfdev_slist_check_add(node, newn)))
        return;
#endif

    newn->next = node->next;
    node->next = newn;
}

/**
 * bfdev_slist_del - delete a node from slist.
 * @head: the head of the slist.
 * @node: the element to delete from the slist.
 */
static inline void
bfdev_slist_del(bfdev_slist_head_t *head, bfdev_slist_head_t *node)
{
    bfdev_slist_head_t *walk = head;

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
bfdev_slist_check_empty(const bfdev_slist_head_t *head)
{
    return !head->next;
}

/**
 * bfdev_slist_check_first - check whether the node is a header.
 * @head: the head of the slist.
 * @node: the entry to test.
 */
static inline bool
bfdev_slist_check_first(const bfdev_slist_head_t *head,
                        const bfdev_slist_head_t *node)
{
    return head->next == node;
}

/**
 * bfdev_slist_check_next - check whether the node is a ending.
 * @node: the node to check.
 */
static inline bool
bfdev_slist_check_end(const bfdev_slist_head_t *node)
{
    return !node->next;
}

/**
 * bfdev_slist_check_another - check whether has another node.
 * @head: slist head to check.
 * @node: the unique node.
 */
static inline bool
bfdev_slist_check_another(const bfdev_slist_head_t *head,
                          const bfdev_slist_head_t *node)
{
    return head->next == node && node->next == NULL;
}

/**
 * bfdev_slist_replace - replace a linked slist node with an external node.
 * @head: the head of the slist.
 * @oldn: the element to be replaced.
 * @newn: the new element to insert.
 */
static inline void
bfdev_slist_replace(bfdev_slist_head_t *head, bfdev_slist_head_t *oldn,
                    bfdev_slist_head_t *newn)
{
    bfdev_slist_head_t *walk = head;

    while (walk->next != oldn)
        walk = walk->next;

    walk->next = newn;
    newn->next = oldn->next;
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
 * @member: the name of the slist head within the struct.
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
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_from_safe(pos, tmp, member) \
    for ((tmp) = bfdev_slist_next_entry(pos, member); (pos); (pos) = (tmp), \
         ((tmp) && ((tmp) = bfdev_slist_next_entry(pos, member))))

/**
 * bfdev_slist_for_each_entry_continue_safe - continue slist iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the slist head within the struct.
 */
#define bfdev_slist_for_each_entry_continue_safe(pos, tmp, member) \
    for ((pos) = bfdev_slist_next_entry(pos, member), \
         (tmp) = bfdev_slist_next_entry(pos, member); (pos); (pos) = (tmp), \
         ((tmp) && ((tmp) = bfdev_slist_next_entry(pos, member))))

BFDEV_END_DECLS

#endif  /* _BFDEV_SLIST_H_ */
