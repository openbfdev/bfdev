/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HLIST_H_
#define _BFDEV_HLIST_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_hlist_head bfdev_hlist_head_t;
typedef struct bfdev_hlist_node bfdev_hlist_node_t;

struct bfdev_hlist_node {
    bfdev_hlist_node_t *next;
    bfdev_hlist_node_t **pprev;
};

struct bfdev_hlist_head {
    bfdev_hlist_node_t *node;
};

#define BFDEV_HLIST_HEAD_STATIC() { \
    .node = NULL, \
}

#define BFDEV_HLIST_HEAD_INIT() \
    (bfdev_hlist_head_t) BFDEV_HLIST_HEAD_STATIC()

#define BFDEV_HLIST_HEAD(name) \
    bfdev_hlist_head_t name = BFDEV_HLIST_HEAD_INIT()

#ifdef BFDEV_DEBUG_HLIST
extern bool
bfdev_hlist_check_head_add(bfdev_hlist_head_t *head, bfdev_hlist_node_t *newn);

extern bool
bfdev_hlist_check_next_add(bfdev_hlist_node_t *next, bfdev_hlist_node_t *newn);

extern bool
bfdev_hlist_check_prev_add(bfdev_hlist_node_t *prev, bfdev_hlist_node_t *newn);

extern bool
bfdev_hlist_check_del(bfdev_hlist_node_t *node);
#endif

/**
 * bfdev_hlist_head_init - initialize a bfdev_hlist_head structure.
 * @head: hlist_head structure to be initialized.
 */
static inline void
bfdev_hlist_head_init(bfdev_hlist_head_t *head)
{
    *head = BFDEV_HLIST_HEAD_INIT();
}

/**
 * bfdev_hlist_node_init - initialize a bfdev_hlist_node structure.
 * @node: bfdev_hlist_node structure to be initialized.
 */
static inline void
bfdev_hlist_node_init(bfdev_hlist_node_t *node)
{
    node->pprev = NULL;
    node->next = NULL;
}

/**
 * bfdev_hlist_head_add - add a new entry at the beginning of the hlist.
 * @head: hlist head to add it after.
 * @newn: new entry to be added.
 */
static inline void
bfdev_hlist_head_add(bfdev_hlist_head_t *head, bfdev_hlist_node_t *newn)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_check_head_add(head, newn)))
        return;
#endif

    newn->next = head->node;
    newn->pprev = &head->node;

    if (head->node)
        head->node->pprev = &newn->next;
    head->node = newn;
}

/**
 * bfdev_hlist_next_add - add a new entry before the one specified.
 * @node: hlist node to add it after, which must be non-NULL.
 * @newn: new entry to be added.
 */
static inline void
bfdev_hlist_next_add(bfdev_hlist_node_t *node, bfdev_hlist_node_t *newn)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_check_next_add(node, newn)))
        return;
#endif

    newn->pprev = &node->next;
    newn->next = node->next;

    if (newn->next)
        newn->next->pprev = &node->next;
    node->next = newn;
}

/**
 * bfdev_hlist_prev_add - add a new entry before the one specified.
 * @node: hlist node to add it before, which must be non-NULL.
 * @newn: new entry to be added.
 */
static inline void
bfdev_hlist_prev_add(bfdev_hlist_node_t *node, bfdev_hlist_node_t *newn)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_check_prev_add(node, newn)))
        return;
#endif

    newn->pprev = node->pprev;
    newn->next = node;
    node->pprev = &node->next;
    *node->pprev = newn;
}

/**
 * bfdev_hlist_deluf - delete the specified bfdev_hlist_node from its hlist (unsafe).
 * @node: the element to delete from the hlist.
 */
static inline void
bfdev_hlist_deluf(bfdev_hlist_node_t *node)
{
    bfdev_hlist_node_t **pprev = node->pprev;
    bfdev_hlist_node_t *next = node->next;

    if (next)
        next->pprev = pprev;
    *pprev = next;
}

/**
 * bfdev_hlist_del - delete the specified bfdev_hlist_node from its hlist.
 * @node: the element to delete from the hlist.
 */
static inline void
bfdev_hlist_del(bfdev_hlist_node_t *node)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_check_del(node)))
        return;
#endif

    bfdev_hlist_deluf(node);
    node->next = BFDEV_POISON_HLIST1;
    node->pprev = BFDEV_POISON_HLIST2;
}

/**
 * bfdev_hlist_check_empty - check whether the node is head.
 * @head: hlist head to check.
 */
static inline bool
bfdev_hlist_check_empty(const bfdev_hlist_head_t *head)
{
    return !head->node;
}

/**
 * bfdev_hlist_check_first - check whether the node is a header.
 * @head: the head of the hlist.
 * @node: the entry to test.
 */
static inline bool
bfdev_hlist_check_first(const bfdev_hlist_head_t *head,
                        const bfdev_hlist_node_t *node)
{
    return head->node == node;
}

/**
 * bfdev_hlist_check_end - check whether the node is a ending.
 * @node: the entry to test.
 */
static inline bool
bfdev_hlist_check_end(const bfdev_hlist_node_t *node)
{
    return !node->next;
}

/**
 * bfdev_hlist_check_another - check whether has another node.
 * @head: hlist head to check.
 * @node: the unique node.
 */
static inline bool
bfdev_hlist_check_another(const bfdev_hlist_head_t *head,
                          const bfdev_hlist_node_t *node)
{
    return head->node == node && node->next == NULL;
}

/**
 * bfdev_hlist_check_unhashed - check whether the node is reinitialized.
 * @node: hlist node to check.
 */
static inline bool
bfdev_hlist_check_unhashed(const bfdev_hlist_node_t *node)
{
    return !node->pprev;
}

/**
 * bfdev_hlist_head_replace - replace a hlist head with an external head.
 * @oldn: bfdev_hlist_head for old hlist.
 * @newn: bfdev_hlist_head for new hlist.
 */
static inline void
bfdev_hlist_head_replace(bfdev_hlist_head_t *oldn, bfdev_hlist_head_t *newn)
{
    newn->node = oldn->node;
    oldn->node = NULL;
    if (newn->node)
        newn->node->pprev = &newn->node;
}

/**
 * bfdev_hlist_replace - replace a hlist node with an external node.
 * @old: bfdev_hlist_head for old hlist.
 * @newn: bfdev_hlist_head for new hlist.
 */
static inline void
bfdev_hlist_replace(bfdev_hlist_node_t *oldn, bfdev_hlist_node_t *newn)
{
    newn->next = oldn->next;
    newn->pprev = oldn->pprev;
    if (newn->next)
        newn->next->pprev = &newn->next;
    *newn->pprev = newn;
}

/**
 * bfdev_hlist_del_init -  deletes entry from hlist and reinitialize it.
 * @node: the element to delete from the hlist.
 */
static inline void
bfdev_hlist_del_init(bfdev_hlist_node_t *node)
{
    bfdev_hlist_deluf(node);
    bfdev_hlist_node_init(node);
}

/**
 * bfdev_hlist_entry - get the struct for this entry.
 * @ptr: the &bfdev_hlist_node_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the hlist_head within the struct.
 */
#define bfdev_hlist_entry(ptr, type, member) \
    bfdev_container_of_safe(ptr, type, member)

/**
 * bfdev_hlist_first_entry - get the first element from a hlist.
 * @ptr: the hlist head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the hlist_head within the struct.
 */
#define bfdev_hlist_first_entry(ptr, type, member) \
    bfdev_hlist_entry((ptr)->node, type, member)

/**
 * bfdev_hlist_next_entry - get the next element in hlist.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_next_entry(pos, member) \
    bfdev_hlist_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * bfdev_hlist_for_each - iterate over a hlist.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @head: the head for your hlist.
 */
#define bfdev_hlist_for_each(pos, head) \
    for ((pos) = (head)->node; (pos); (pos) = (pos)->next)

/**
 * bfdev_hlist_for_each_from - iterate over a hlist from the current point.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 */
#define bfdev_hlist_for_each_from(pos) \
    for (; (pos); (pos) = (pos)->next)

/**
 * bfdev_hlist_for_each_continue - continue iteration over a hlist.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 */
#define bfdev_hlist_for_each_continue(pos) \
    for ((void)((pos) && ((pos) = (pos)->next)); (pos); (pos) = (pos)->next)

/**
 * bfdev_hlist_for_each_safe - iterate over a hlist safe against removal of hlist entry.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 * @head: the head for your hlist.
 */
#define bfdev_hlist_for_each_safe(pos, tmp, head) \
    for ((pos) = (head)->node; (pos) && ({(tmp) = (pos)->next; 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_hlist_for_each_from_safe - iterate over a hlist safe against removal of hlist entry from the current point.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 */
#define bfdev_hlist_for_each_from_safe(pos, tmp) \
    for (; (pos) && ({(tmp) = (pos)->next; 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_hlist_for_each_continue_safe - continue hlist iteration safe against removal.
 * @pos: the &bfdev_hlist_node_t to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 */
#define bfdev_hlist_for_each_continue_safe(pos, tmp) \
    for ((void)((pos) && ((pos) = (pos)->next)); \
         (pos) && ({(tmp) = (pos)->next; 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_hlist_for_each_entry - iterate over hlist of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your hlist.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry(pos, head, member) \
    for ((pos) = bfdev_hlist_first_entry(head, typeof(*(pos)), member); \
         (pos); (pos) = bfdev_hlist_next_entry(pos, member))

/**
 * bfdev_hlist_for_each_entry_from - continue iteration over hlist of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_from(pos, member) \
    for (; (pos); (pos) = bfdev_hlist_next_entry(pos, member))

/**
 * bfdev_hlist_for_each_entry_continue - continue iteration over hlist of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_continue(pos, member) \
    for ((void)((pos) && ((pos) = bfdev_hlist_next_entry(pos, member))); \
         (pos); (pos) = bfdev_hlist_next_entry(pos, member))

/**
 * bfdev_hlist_for_each_entry_safe - iterate over hlist of given type safe against removal of hlist entry
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage
 * @head: the head for your hlist.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_hlist_first_entry(head, typeof(*(pos)), member); \
         (pos) && ({(tmp) = bfdev_hlist_next_entry(pos, member); 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = bfdev_hlist_next_entry(tmp, member))))

/**
 * bfdev_hlist_for_each_entry_from_safe - iterate over hlist from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_from_safe(pos, tmp, member) \
    for (; (pos) && ({(tmp) = bfdev_hlist_next_entry(pos, member); 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = bfdev_hlist_next_entry(pos, member))))

/**
 * bfdev_hlist_for_each_entry_continue_safe - continue hlist iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_continue_safe(pos, tmp, member) \
    for ((void)((pos) && ((pos) = bfdev_hlist_next_entry(pos, member))); \
         (pos) && ({(tmp) = bfdev_hlist_next_entry(pos, member); 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = bfdev_hlist_next_entry(pos, member))))

BFDEV_END_DECLS

#endif /* _BFDEV_HLIST_H_ */
