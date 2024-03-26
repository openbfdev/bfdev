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
ilist_integrity_check(bfdev_list_head_t *head, bfdev_list_head_t *prev,
                      bfdev_list_head_t *next)
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
ilist_list_check(bfdev_list_head_t *head)
{
    bfdev_list_head_t *prev = head, *next = head->next;
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
ilist_head_check(bfdev_ilist_head_t *ihead)
{
    bool success;

    success = ilist_list_check(&ihead->node_list);
    if (success && !bfdev_ilist_head_empty(ihead))
        ilist_list_check(&bfdev_ilist_first(ihead)->index_list);

    return success;
}
#endif

export void
bfdev_ilist_add(bfdev_ilist_head_t *ihead, bfdev_ilist_node_t *inode,
                bfdev_ilist_cmp_t cmp, void *pdata)
{
    bfdev_ilist_node_t *walk, *first, *prev;
    bfdev_list_head_t *next;

#ifdef BFDEV_DEBUG_ILIST
    if (bfdev_unlikely(!ilist_head_check(ihead)))
        return;
#endif

    /* Check if node is initialized */
    if (!bfdev_list_check_empty(&inode->node_list) ||
        !bfdev_list_check_empty(&inode->index_list))
        return;

    /* Direct insertion of new nodes */
    next = &ihead->node_list;
    if (bfdev_ilist_head_empty(ihead))
        goto finish;

    /* Traverse to find a suitable insertion point */
    first = walk = bfdev_ilist_first(ihead);
    prev = NULL;

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

export void
bfdev_ilist_del(bfdev_ilist_head_t *ihead, bfdev_ilist_node_t *inode)
{
    bfdev_ilist_node_t *next;

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
