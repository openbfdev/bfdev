/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LLIST_H_
#define _BFDEV_LLIST_H_

#include <bfdev/config.h>
#include <bfdev/slist.h>
#include <bfdev/atomic.h>
#include <bfdev/cmpxchg.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_llist_split() - insert a range into the linked list.
 * @head: the head for your lock-less list.
 * @node: first entry in batch to be added.
 * @end: last entry in batch to be added.
 *
 * Return whether list is empty before adding.
 */
extern bool
bfdev_llist_split(bfdev_slist_head_t *head, bfdev_slist_head_t *node,
                  bfdev_slist_head_t *end);

/**
 * bfdev_llist_del() - delete the first entry of lock-less list.
 * @head: the head for your lock-less list.
 */
extern bfdev_slist_head_t *
bfdev_llist_del(bfdev_slist_head_t *head);

/**
 * bfdev_llist_add() - add a new entry.
 * @head: the head for your lock-less list.
 * @node: new entry to be added.
 *
 * Return whether list is empty before adding.
 */
static inline bool
bfdev_llist_add(bfdev_slist_head_t *head, bfdev_slist_head_t *node)
{
    return bfdev_llist_split(head, node, node);
}

/**
 * bfdev_llist_destroy() - delete all entries from lock-less list.
 * @head: the head of lock-less list to delete all entries
 */
static inline bfdev_slist_head_t *
bfdev_llist_destroy(bfdev_slist_head_t *head)
{
    return (void *)bfdev_xchg((bfdev_atomic_t *)&head->next, (bfdev_atomic_t)NULL);
}

BFDEV_END_DECLS

#endif /* _BFDEV_LLIST_H_ */
