/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ILIST_H_
#define _BFDEV_ILIST_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_ilist_head bfdev_ilist_head_t;
typedef struct bfdev_ilist_node bfdev_ilist_node_t;

struct bfdev_ilist_head {
    bfdev_list_head_t node_list;
};

struct bfdev_ilist_node {
    bfdev_list_head_t node_list;
    bfdev_list_head_t index_list;
};

#define BFDEV_ILIST_HEAD_STATIC(HEAD) { \
    .node_list = BFDEV_LIST_HEAD_STATIC(&(HEAD)->node_list), \
}

#define BFDEV_ILIST_HEAD_INIT(head) \
    (bfdev_ilist_head_t) BFDEV_ILIST_HEAD_STATIC(head)

#define BFDEV_ILIST_HEAD(name) \
    bfdev_ilist_head_t name = BFDEV_ILIST_HEAD_STATIC(&name)

#define BFDEV_ILIST_NODE_INIT(node) (bfdev_ilist_node_t) {      \
    .node_list = BFDEV_LIST_HEAD_INIT(&(node)->node_list),      \
    .index_list = BFDEV_LIST_HEAD_INIT(&(node)->index_list),    \
}

BFDEV_CALLBACK_CMP(
    bfdev_ilist_cmp_t,
    const bfdev_ilist_node_t *
);

/**
 * bfdev_ilist_add() - insert a new index list node.
 * @ihead: the index head to be insert.
 * @inode: the index node to insert.
 */
extern void
bfdev_ilist_add(bfdev_ilist_head_t *ihead, bfdev_ilist_node_t *inode,
                bfdev_ilist_cmp_t cmp, void *pdata);

/**
 * bfdev_ilist_del() - delete a node form index head.
 * @ihead: the index head to be delete.
 * @inode: the index node to delete.
 */
extern void
bfdev_ilist_del(bfdev_ilist_head_t *ihead, bfdev_ilist_node_t *inode);

/**
 * bfdev_ilist_head_init() - initialize a bfdev_ilist_head structure.
 * @head: bfdev_ilist_head structure to be initialized.
 */
static inline void
bfdev_ilist_head_init(bfdev_ilist_head_t *ihead)
{
    *ihead = BFDEV_ILIST_HEAD_INIT(ihead);
}

/**
 * bfdev_ilist_node_init() - initialize a bfdev_ilist_node structure.
 * @inode: bfdev_ilist_node structure to be initialized.
 */
static inline void
bfdev_ilist_node_init(bfdev_ilist_node_t *inode)
{
    *inode = BFDEV_ILIST_NODE_INIT(inode);
}

/**
 * bfdev_ilist_first() - return the first node.
 * @ihead: the &bfdev_ilist_head_t pointer
 */
static inline bfdev_ilist_node_t *
bfdev_ilist_first(const bfdev_ilist_head_t *ihead)
{
    return bfdev_list_first_entry(&ihead->node_list, bfdev_ilist_node_t, node_list);
}

/**
 * bfdev_ilist_last() - return the last node.
 * @ihead:the &bfdev_ilist_head_t pointer
 */
static inline bfdev_ilist_node_t *
bfdev_ilist_last(const bfdev_ilist_head_t *ihead)
{
    return bfdev_list_last_entry(&ihead->node_list, bfdev_ilist_node_t, node_list);
}

/**
 * bfdev_list_check_empty() - check whether a head is empty.
 * @ihead: list head to check.
 */
static inline bool
bfdev_ilist_head_empty(bfdev_ilist_head_t *ihead)
{
    return bfdev_list_check_empty(&ihead->node_list);
}

/**
 * bfdev_list_check_empty() - check whether a node index is empty.
 * @inode: list node to check.
 */
static inline bool
bfdev_ilist_node_empty(bfdev_ilist_node_t *inode)
{
    return bfdev_list_check_empty(&inode->index_list);
}

/**
 * bfdev_ilist_entry - get the struct for this entry.
 * @ptr: the &bfdev_ilist_head_t pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the bfdev_ilist_head within the struct.
 */
#define bfdev_ilist_entry(ptr, type, member) \
    bfdev_container_of(ptr, type, member)

/**
 * bfdev_ilist_for_each - iterate over a list.
 * @head: the head for your list.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 */
#define bfdev_ilist_for_each(pos, head) \
    bfdev_list_for_each(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse - iterate over a list backwards.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse(pos, head) \
    bfdev_list_for_each_reverse(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_from - iterate over a list from the current point.
 * @head: the head for your list.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 */
#define bfdev_ilist_for_each_from(pos, head) \
    bfdev_list_for_each_from(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_from - iterate over a list backwards from the current point.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_from(pos, head) \
    bfdev_list_for_each_reverse_from(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_continue - continue iteration over a list.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_continue(pos, head) \
    bfdev_list_for_each_continue(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_continue - continue iteration over a list backwards.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_continue(pos, head) \
    bfdev_list_for_each_reverse_continue(pos, &(head)->node_list)

/**
 * bfdev_ilist_for_each_safe - iterate over a list safe against removal of list entry.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_safe(pos, tmp, head) \
    bfdev_list_for_each_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_safe(pos, tmp, head) \
    bfdev_list_for_each_reverse_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_from_safe - iterate over a list safe against removal of list entry from the current point.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_from_safe(pos, tmp, head) \
    bfdev_list_for_each_from_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_from_safe - iterate backwards over list safe against removal from the current point.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_reverse_from_safe(pos, tmp, head) \
    bfdev_list_for_each_reverse_from_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_continue_safe - continue list iteration safe against removal.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
 * @tmp: another bfdev_list_head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_ilist_for_each_continue_safe(pos, tmp, head) \
    bfdev_list_for_each_continue_safe(pos, tmp, &(head)->node_list)

/**
 * bfdev_ilist_for_each_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the &bfdev_list_head_t to use as a loop cursor.
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
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_reverse_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_from_safe - iterate over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_from_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_from_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_from_safe - iterate backwards over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_from_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_reverse_from_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_continue_safe - continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_continue_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_continue_safe(pos, tmp, &(head)->node_list, member.node_list)

/**
 * bfdev_ilist_for_each_entry_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the bfdev_list_head within the struct.
 */
#define bfdev_ilist_for_each_entry_reverse_continue_safe(pos, tmp, head, member) \
    bfdev_list_for_each_entry_reverse_continue_safe(pos, tmp, &(head)->node_list, member.node_list)

BFDEV_END_DECLS

#endif /* _BFDEV_ILIST_H_ */
