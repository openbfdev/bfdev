/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-ilist"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/ilist.h>
#include <bfdev/log.h>
#include <export.h>

#ifdef BFDEV_DEBUG_ILIST
static inline bool
ilist_integrity_check(struct bfdev_list_head *head, struct bfdev_list_head *prev,
                      struct bfdev_list_head *next)
{
    if (bfdev_unlikely(prev->next != next)) {
        bfdev_log_err(
            "ilist corruption head (%p) (%p) prev->next"
            " should be next (%p), but was (%p)\n",
            head, prev, next, prev->next
        );
        return false;
    }

    if (bfdev_unlikely(next->prev != prev)) {
        bfdev_log_err(
            "ilist corruption head (%p) (%p) next->prev"
            " should be prev (%p), but was (%p)\n",
            head, next, prev, next->prev
        );
        return false;
    }

    return true;
}

static inline bool
ilist_list_check(struct bfdev_list_head *head)
{
    struct bfdev_list_head *prev = head, *next = head->next;
    bool success;

    success = ilist_integrity_check(head, prev, next);
    while (success && next != head) {
        prev = next;
        next = prev->next;
        success = ilist_integrity_check(head, prev, next);
    }

    return success;
}

static inline bool
ilist_head_check(struct bfdev_ilist_head *ihead)
{
    bool success;

    success = ilist_list_check(&ihead->node_list);
    if (success && !bfdev_ilist_head_empty(ihead))
        ilist_list_check(&bfdev_ilist_first(ihead)->index_list);

    return success;
}
#endif

/**
 * ilist_add - insert a new index list node.
 * @ihead: the index head to be insert.
 * @inode: the index node to insert.
 */
export void
bfdev_ilist_add(struct bfdev_ilist_head *ihead, struct bfdev_ilist_node *inode,
                bfdev_ilist_cmp_t cmp, const void *pdata)
{
    struct bfdev_ilist_node *walk, *first, *prev = NULL;
    struct bfdev_list_head *next = &ihead->node_list;

#ifdef BFDEV_DEBUG_ILIST
    if (bfdev_unlikely(!ilist_head_check(ihead)))
        return;
#endif

    /* Check if node is initialized */
    if (!bfdev_list_check_empty(&inode->node_list) ||
        !bfdev_list_check_empty(&inode->index_list))
        return;

    /* Direct insertion of new nodes */
    if (bfdev_ilist_head_empty(ihead))
        goto finish;

    /* Traverse to find a suitable insertion point */
    first = walk = bfdev_ilist_first(ihead);
    do {
        if (cmp(inode, walk, pdata) < 0) {
            next = &walk->node_list;
            break;
        }
        prev = walk;
        walk = bfdev_list_next_entry(walk, index_list);
    } while (walk != first);

    /* Whether to insert the index linked list */
    if (!prev || cmp(prev, inode, pdata))
        bfdev_list_add_prev(&walk->index_list, &inode->index_list);

finish:
    bfdev_list_add_prev(next, &inode->node_list);

#ifdef BFDEV_DEBUG_ILIST
    ilist_head_check(ihead);
#endif
}

/**
 * ilist_del - delete a node form index head.
 * @ihead: the index head to be delete.
 * @inode: the index node to delete.
 */
export void
bfdev_ilist_del(struct bfdev_ilist_head *ihead, struct bfdev_ilist_node *inode)
{
    struct bfdev_ilist_node *next;

#ifdef BFDEV_DEBUG_ILIST
    if (bfdev_unlikely(!ilist_head_check(ihead)))
        return;
#endif

    /* Check if index list is inserted */
    if (!bfdev_ilist_node_empty(inode)) {
        if (inode->node_list.next != &ihead->node_list) {
            next = bfdev_list_next_entry(inode, index_list);
            if (bfdev_ilist_node_empty(next))
                bfdev_list_add(&inode->index_list, &inode->index_list);
        }
        bfdev_list_del_init(&inode->index_list);
    }

    bfdev_list_del_init(&inode->node_list);

#ifdef BFDEV_DEBUG_ILIST
    ilist_head_check(ihead);
#endif
}
