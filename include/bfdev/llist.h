/* SPDX-License-Identifier: GPL-2.0-or-later */
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

extern bool bfdev_llist_split(struct bfdev_slist_head *head, struct bfdev_slist_head *node, struct bfdev_slist_head *end);
extern struct bfdev_slist_head *bfdev_llist_del(struct bfdev_slist_head *head);

static inline bool
bfdev_llist_add(struct bfdev_slist_head *head, struct bfdev_slist_head *node)
{
    return bfdev_llist_split(head, node, node);
}

static inline struct bfdev_slist_head *
bfdev_llist_destroy(struct bfdev_slist_head *head)
{
    return (void *)bfdev_xchg((bfdev_atomic_t *)&head->next, (bfdev_atomic_t)NULL);
}

BFDEV_END_DECLS

#endif /* _BFDEV_LLIST_H_ */
