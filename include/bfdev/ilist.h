/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ILIST_H_
#define _BFDEV_ILIST_H_

#include <bfdev/config.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

struct bfdev_ilist_head {
    struct bfdev_list_head node_list;
};

struct bfdev_ilist_node {
    struct bfdev_list_head node_list;
    struct bfdev_list_head index_list;
};

#define BFDEV_ILIST_HEAD_STATIC(name) \
    {BFDEV_LIST_HEAD_STATIC((name).node_list)}

#define BFDEV_ILIST_HEAD_INIT(name) \
    (struct bfdev_ilist_head) BFDEV_ILIST_HEAD_STATIC(name)

#define BFDEV_ILIST_HEAD(name) \
    struct bfdev_ilist_head name = BFDEV_ILIST_HEAD_STATIC(name)

#define BFDEV_ILISI_NODE_INIT(name, index)                      \
    (struct bfdev_ilist_node) {                                 \
        .node_list = BFDEV_LIST_HEAD_INIT((node).node_list),    \
        .index_list = BFDEV_LIST_HEAD_INIT((node).index_list),  \
    }

typedef long (*bfdev_ilist_cmp_t)(struct bfdev_ilist_node *inodea, struct bfdev_ilist_node *inodeb, const void *pdata);
extern void bfdev_ilist_add(struct bfdev_ilist_head *ihead, struct bfdev_ilist_node *inode, bfdev_ilist_cmp_t cmp, const void *pdata);
extern void bfdev_ilist_del(struct bfdev_ilist_head *ihead, struct bfdev_ilist_node *inode);

/**
 * bfdev_ilist_head_init - initialize a bfdev_ilist_head structure.
 * @head: bfdev_ilist_head structure to be initialized.
 */
static inline void
bfdev_ilist_head_init(struct bfdev_ilist_head *ihead)
{
    bfdev_list_head_init(&ihead->node_list);
}

static inline void
bfdev_ilist_node_init(struct bfdev_ilist_node *inode)
{
    bfdev_list_head_init(&inode->node_list);
    bfdev_list_head_init(&inode->index_list);
}

/**
 * bfdev_ilist_first - return the first node.
 * @ihead: the &struct bfdev_ilist_head pointer
 */
static inline struct bfdev_ilist_node *
bfdev_ilist_first(const struct bfdev_ilist_head *ihead)
{
    return bfdev_list_first_entry(&ihead->node_list, struct bfdev_ilist_node, node_list);
}

/**
 * bfdev_ilist_last - return the last node.
 * @ihead:the &struct bfdev_ilist_head pointer
 */
static inline struct bfdev_ilist_node *
bfdev_ilist_last(const struct bfdev_ilist_head *ihead)
{
    return bfdev_list_last_entry(&ihead->node_list, struct bfdev_ilist_node, node_list);
}

/**
 * bfdev_list_check_empty - check whether a head is empty.
 * @ihead: list head to check.
 */
static inline bool
bfdev_ilist_head_empty(struct bfdev_ilist_head *ihead)
{
    return bfdev_list_check_empty(&ihead->node_list);
}

/**
 * bfdev_list_check_empty - check whether a node index is empty.
 * @inode: list node to check.
 */
static inline bool
bfdev_ilist_node_empty(struct bfdev_ilist_node *inode)
{
    return bfdev_list_check_empty(&inode->index_list);
}

/**
 * bfdev_ilist_entry - get the struct for this entry.
 * @ptr: the &struct bfdev_ilist_head pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_ilist_head within the struct.
 */
#define bfdev_ilist_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_ilist_for_each - iterate over a list.
 * @head: the head for your list.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 */
#define bfdev_ilist_for_each(pos, head) \
    bfdev_list_for_each(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse - iterate over a list backwards.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse(pos, head) \
    bfdev_list_for_each_reverse(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_from - iterate over a list from the current point.
 * @head: the head for your list.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 */
#define bfdev_ilist_for_each_from(pos, head) \
    bfdev_list_for_each_from(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_from - iterate over a list backwards from the current point.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_from(pos, head) \
    bfdev_list_for_each_reverse_from(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_continue - continue iteration over a list.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_continue(pos, head) \
    bfdev_list_for_each_continue(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_continue - continue iteration over a list backwards.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_continue(pos, head) \
    bfdev_list_for_each_reverse_continue(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_safe - iterate over a list safe against removal of list entry.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_safe(pos, tmp, head) \
    bfdev_list_for_each_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_safe(pos, tmp, head) \
    bfdev_list_for_each_reverse_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_from_safe - iterate over a list safe against removal of list entry from the current point.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_from_safe(pos, tmp, head) \
    bfdev_list_for_each_from_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_from_safe - iterate backwards over list safe against removal from the current point.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_from_safe(pos, tmp, head) \
    bfdev_list_for_each_reverse_from_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_continue_safe - continue list iteration safe against removal.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_continue_safe(pos, tmp, head) \
    bfdev_list_for_each_continue_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the &struct bfdev_list_head to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_continue_safe(pos, tmp, head) \
    bfdev_list_for_each_reverse_continue_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_entry - iterate over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry(pos, head, member) \
    bfdev_list_for_each_entry(pos, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse(pos, head, member) \
    bfdev_list_for_each_entry_reverse(pos, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_from - iterate over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_from(pos, head, member) \
    bfdev_list_for_each_entry_from(pos, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_from - iterate backwards over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_from(pos, head, member) \
    bfdev_list_for_each_entry_reverse_from(pos, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_continue - continue iteration over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_continue(pos, head, member) \
    bfdev_list_for_each_entry_continue(pos, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_continue - iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_continue(pos, head, member) \
    bfdev_list_for_each_entry_reverse_continue(pos, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_safe - iterate over list of given type safe against removal of list entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_reverse_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_from_safe - iterate over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_from_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_from_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_from_safe - iterate backwards over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_from_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_reverse_from_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_continue_safe - continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_continue_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_continue_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_continue_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_reverse_continue_safe(pos, tmp, &(head)->node_list, member.node_list)

BFDEV_END_DECLS

#endif /* _BFDEV_ILIST_H_ */
