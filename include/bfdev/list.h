/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LIST_H_
#define _BFDEV_LIST_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/container.h>
#include <bfdev/poison.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_list_head bfdev_list_head_t;

struct bfdev_list_head {
    bfdev_list_head_t *prev;
    bfdev_list_head_t *next;
};

#define BFDEV_LIST_HEAD_STATIC(name) \
    {&(name), &(name)}

#define BFDEV_LIST_HEAD_INIT(name) \
    (bfdev_list_head_t) BFDEV_LIST_HEAD_STATIC(name)

#define BFDEV_LIST_HEAD(name) \
    bfdev_list_head_t name = BFDEV_LIST_HEAD_INIT(name)

BFDEV_CALLBACK_CMP(
    bfdev_list_cmp_t,
    const bfdev_list_head_t *
);

extern void
bfdev_list_sort(bfdev_list_head_t *head,
                bfdev_list_cmp_t cmp, void *pdata);

#ifdef BFDEV_DEBUG_LIST
extern bool
bfdev_list_check_add(bfdev_list_head_t *prev, bfdev_list_head_t *next,
                     bfdev_list_head_t *node);

extern bool
bfdev_list_check_del(bfdev_list_head_t *node);
#endif

static inline void
bfdev_list_insert(bfdev_list_head_t *prev, bfdev_list_head_t *next,
                  bfdev_list_head_t *node)
{
#ifdef BFDEV_DEBUG_LIST
    if (bfdev_unlikely(!bfdev_list_check_add(prev, next, node)))
        return;
#endif

    prev->next = node;
    next->prev = node;
    node->prev = prev;
    node->next = next;
}

/**
 * bfdev_list_head_init - initialize a list head structure.
 * @head: list head structure to be initialized.
 */
static inline void
bfdev_list_head_init(bfdev_list_head_t *head)
{
    head->prev = head;
    head->next = head;
}

/**
 * bfdev_list_add - add a new node next old node.
 * @node: list head to add it next.
 * @newn: new entry to be added.
 */
static inline void
bfdev_list_add(bfdev_list_head_t *node, bfdev_list_head_t *newn)
{
    bfdev_list_insert(node, node->next, newn);
}

/**
 * bfdev_list_add_prev - add a new node after old node.
 * @node: list head to add it prev.
 * @newn: new entry to be added.
 */
static inline void
bfdev_list_add_prev(bfdev_list_head_t *node, bfdev_list_head_t *newn)
{
    bfdev_list_insert(node->prev, node, newn);
}

/**
 * bfdev_list_deluf - deletes entry from list (unsafe).
 * @node: the element to delete from the list.
 */
static inline void
bfdev_list_deluf(bfdev_list_head_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/**
 * bfdev_list_del - deletes entry from list.
 * @node: the element to delete from the list.
 */
static inline void
bfdev_list_del(bfdev_list_head_t *node)
{
#ifdef BFDEV_DEBUG_LIST
    if (bfdev_unlikely(!bfdev_list_check_del(node)))
        return;
#endif

    bfdev_list_deluf(node);
    node->next = (bfdev_list_head_t *)BFDEV_POISON_LIST1;
    node->prev = (bfdev_list_head_t *)BFDEV_POISON_LIST2;
}

/**
 * bfdev_list_check_empty - check whether a list is empty.
 * @head: list head to check.
 */
static inline bool
bfdev_list_check_empty(const bfdev_list_head_t *head)
{
    return head->next == head;
}

/**
 * bfdev_list_check_head - check whether the node is head.
 * @head: the head of the list
 * @list: the entry to test
 */
static inline bool
bfdev_list_check_head(const bfdev_list_head_t *head,
                      const bfdev_list_head_t *node)
{
    return head == node;
}

/**
 * bfdev_list_check_first - check whether the node is a header.
 * @head: the head of the list.
 * @node: the entry to test.
 */
static inline bool
bfdev_list_check_first(const bfdev_list_head_t *head,
                       const bfdev_list_head_t *node)
{
    return node->prev == head;
}

/**
 * bfdev_list_check_end - check whether the node is a ending.
 * @head: the head of the list.
 * @node: the entry to test.
 */
static inline bool
bfdev_list_check_end(const bfdev_list_head_t *head,
                     const bfdev_list_head_t *node)
{
    return node->next == head;
}

/**
 * bfdev_list_check_another - check whether has another node.
 * @head: list head to check.
 * @node: the unique node.
 */
static inline bool
bfdev_list_check_another(const bfdev_list_head_t *head,
                         const bfdev_list_head_t *node)
{
    return head->next == node && head->prev == node;
}

static inline void
bfdev_list_relocate(bfdev_list_head_t *prev, bfdev_list_head_t *next,
                    bfdev_list_head_t *list)
{
    bfdev_list_head_t *first = list->next;
    bfdev_list_head_t *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
 * bfdev_list_splice - join two lists, this is designed for stacks.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice(bfdev_list_head_t *head, bfdev_list_head_t *list)
{
    if (!bfdev_list_check_empty(list))
        bfdev_list_relocate(head, head->next, list);
}

/**
 * bfdev_list_splice_prev - join two lists, each list being a queue.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice_prev(bfdev_list_head_t *head, bfdev_list_head_t *list)
{
    if (!bfdev_list_check_empty(list))
        bfdev_list_relocate(head->prev, head, list);
}

/**
 * bfdev_list_replace - replace a list node with an external node.
 * @oldn: the element to be replaced.
 * @newn: the new element to insert.
 */
static inline void
bfdev_list_replace(bfdev_list_head_t *oldn, bfdev_list_head_t *newn)
{
    newn->prev = oldn->prev;
    newn->next = oldn->next;
    newn->prev->next = newn;
    newn->next->prev = newn;
}

/**
 * bfdev_list_move - move the node to the next of the node.
 * @head: the head that will precede our entry.
 * @node: the entry to move.
 */
static inline void
bfdev_list_move(bfdev_list_head_t *head, bfdev_list_head_t *node)
{
    bfdev_list_del(node);
    bfdev_list_add(head, node);
}

/**
 * bfdev_list_move_prev - move the node to the prev of the node.
 * @head: the head that will follow our entry.
 * @node: the entry to move.
 */
static inline void
bfdev_list_move_prev(bfdev_list_head_t *head, bfdev_list_head_t *node)
{
    bfdev_list_del(node);
    bfdev_list_add_prev(head, node);
}

/**
 * bfdev_list_swap - replace entry1 with entry2 and re-add entry1 at entry2's position.
 * @node1: the location to place entry2.
 * @node2: the location to place entry1.
 */
static inline void
bfdev_list_swap(bfdev_list_head_t *node1, bfdev_list_head_t *node2)
{
    bfdev_list_head_t *prev = node2->prev;

    bfdev_list_del(node2);
    bfdev_list_replace(node1, node2);

    if (prev == node1)
        prev = node2;
    bfdev_list_add(prev, node1);
}

/**
 * bfdev_list_del_init - deletes entry from list and reinitialize it.
 * @node: the element to delete from the list.
 */
static inline void
bfdev_list_del_init(bfdev_list_head_t *node)
{
    bfdev_list_deluf(node);
    bfdev_list_head_init(node);
}

/**
 * bfdev_list_replace_init - replace old entry by new one and initialize the old one.
 * @oldn: the element to be replaced.
 * @newn: the new element to insert.
 */
static inline void
bfdev_list_replace_init(bfdev_list_head_t *oldn, bfdev_list_head_t *newn)
{
    bfdev_list_replace(oldn, newn);
    bfdev_list_head_init(oldn);
}

/**
 * bfdev_list_splice_init - join two lists and reinitialise the emptied list.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice_init(bfdev_list_head_t *head, bfdev_list_head_t *list)
{
    if (!bfdev_list_check_empty(list)) {
        bfdev_list_splice(head, list);
        bfdev_list_head_init(list);
    }
}

/**
 * bfdev_list_splice_tail_init - join two lists and reinitialise the emptied list.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice_tail_init(bfdev_list_head_t *head, bfdev_list_head_t *list)
{
    if (!bfdev_list_check_empty(list)) {
        bfdev_list_splice_prev(head, list);
        bfdev_list_head_init(list);
    }
}

/**
 * bfdev_list_entry - get the struct for this entry.
 * @ptr: the &struct list head pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_list_entry_check_head - test if the entry points to the head of the list.
 * @pos: the type * to cursor
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_entry_check_head(pos, head, member) \
    (&(pos)->member == (head))

/**
 * bfdev_list_first_entry - get the first element from a list.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_first_entry(ptr, type, member) \
    bfdev_list_entry((ptr)->next, type, member)

/**
 * bfdev_list_last_entry - get the last element from a list.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_last_entry(ptr, type, member) \
    bfdev_list_entry((ptr)->prev, type, member)

/**
 * bfdev_list_next_entry - get the next element in list.
 * @pos: the type * to cursor.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_next_entry(pos, member) \
    bfdev_list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * bfdev_list_prev_entry - get the prev element in list.
 * @pos: the type * to cursor.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_prev_entry(pos, member) \
    bfdev_list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * bfdev_list_first_entry_or_null - get the first element from a list or null.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_first_entry_or_null(ptr, type, member) ({ \
    bfdev_list_head_t *head__ = (ptr); \
    bfdev_list_head_t *pos__ = head__->next; \
    pos__ != head__ ? bfdev_list_entry(pos__, type, member) : NULL; \
})

/**
 * bfdev_list_last_entry_or_null - get the last element from a list or null.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_last_entry_or_null(ptr, type, member) ({ \
    bfdev_list_head_t *head__ = (ptr); \
    bfdev_list_head_t *pos__ = head__->prev; \
    pos__ != head__ ? bfdev_list_entry(pos__, type, member) : NULL; \
})

/**
 * bfdev_list_next_entry_or_null - get the next element in list or null.
 * @pos: the type * to cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_next_entry_or_null(pos, head, member) ({ \
    typeof(*(pos)) *pos__; \
    pos__ = bfdev_list_entry((pos)->member.next, typeof(*(pos)), member); \
    bfdev_list_entry_check_head(pos__, head, member) ? NULL : pos__; \
})

/**
 * bfdev_list_prev_entry_or_null - get the prev element in list or null.
 * @pos: the type * to cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_prev_entry_or_null(pos, head, member) ({ \
    typeof(*(pos)) *pos__; \
    pos__ = bfdev_list_entry((pos)->member.prev, typeof(*(pos)), member); \
    bfdev_list_entry_check_head(pos__, head, member) ? NULL : pos__; \
})

/**
 * bfdev_list_for_each - iterate over a list.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each(pos, head) \
    for ((pos) = (head)->next; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse - iterate over a list backwards.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse(pos, head) \
    for ((pos) = (head)->prev; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_from - iterate over a list from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_from(pos, head) \
    for (; !bfdev_list_check_head(head, pos); (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse_from - iterate over a list backwards from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_from(pos, head) \
    for (; !bfdev_list_check_head(head, pos); (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_continue - continue iteration over a list.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue(pos, head) \
    for ((pos) = (pos)->next; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse_continue - continue iteration over a list backwards.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue(pos, head) \
    for ((pos) = (pos)->prev; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_continue_check - check head and continue iteration over a list.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue_check(pos, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && ((pos) = (pos)->next)); \
         !bfdev_list_check_head(head, pos); (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse_continue_check - check head and continue iteration over a list backwards.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue_check(pos, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && ((pos) = (pos)->prev)); \
         !bfdev_list_check_head(head, pos); (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_safe - iterate over a list safe against removal of list entry.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_safe(pos, tmp, head) \
    for ((pos) = (head)->next, (tmp) = (pos)->next; \
         !bfdev_list_check_head((pos), head); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_safe(pos, tmp, head) \
    for ((pos) = (head)->prev, (tmp) = (pos)->prev; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_from_safe - iterate over a list safe against removal of list entry from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_from_safe(pos, tmp, head) \
    for ((tmp) = (pos)->next; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_from_safe - iterate backwards over list safe against removal from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_from_safe(pos, tmp, head) \
    for ((tmp) = pos->prev; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_continue_safe - continue list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue_safe(pos, tmp, head) \
    for ((pos) = (pos)->next, (tmp) = (pos)->next; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue_safe(pos, tmp, head) \
    for ((pos) = pos->prev, (tmp) = pos->prev; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_continue_check_safe - check head and continue list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue_check_safe(pos, tmp, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && \
         ((pos) = (pos)->next, (tmp) = (pos)->next)); \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_continue_check_safe - check head and continue backwards over list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue_check_safe(pos, tmp, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && \
         ((pos) = pos->prev, (tmp) = pos->prev)); \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_entry - iterate over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry(pos, head, member) \
    for ((pos) = bfdev_list_first_entry(head, typeof(*(pos)), member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse(pos, head, member) \
    for ((pos) = bfdev_list_last_entry(head, typeof(*(pos)), member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_from - iterate over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_from(pos, head, member) \
    for (; !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse_from - iterate backwards over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_from(pos, head, member) \
    for (; !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_continue - continue iteration over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue(pos, head, member) \
    for ((pos) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse_continue - iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue(pos, head, member) \
    for ((pos) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_continue_check - check head and continue iteration over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue_check(pos, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_next_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse_continue_check - check head and iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue_check(pos, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_prev_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_first_entry(head, typeof(*(pos)), member), \
         (tmp) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_last_entry(head, typeof(*pos), member), \
         (tmp) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_from_safe - iterate over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_from_safe(pos, tmp, head, member) \
    for ((tmp) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_from_safe - iterate backwards over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_from_safe(pos, tmp, head, member) \
    for ((tmp) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_continue_safe - continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_next_entry(pos, member), \
         (tmp) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_prev_entry(pos, member), \
         (tmp) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_continue_check_safe - check head and continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue_check_safe(pos, tmp, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_next_entry(pos, member), \
         (tmp) = bfdev_list_next_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_continue_check_safe - check head and continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue_check_safe(pos, tmp, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_prev_entry(pos, member), \
         (tmp) = bfdev_list_prev_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

BFDEV_END_DECLS

#endif /* _BFDEV_LIST_H_ */
