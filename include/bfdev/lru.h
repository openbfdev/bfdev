/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LRU_H_
#define _BFDEV_LRU_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>
#include <bfdev/list.h>
#include <bfdev/hlist.h>
#include <bfdev/bitflags.h>

BFDEV_BEGIN_DECLS

enum bfdev_lru_obtain {
    __BFDEV_LRU_CHANGE = 0,
    __BFDEV_LRU_UNCOMMITTED,

    BFDEV_LRU_CHANGE = BFDEV_BIT(__BFDEV_LRU_CHANGE),
    BFDEV_LRU_UNCOMMITTED = BFDEV_BIT(__BFDEV_LRU_UNCOMMITTED),
};

enum bfdev_lru_flags {
    __BFDEV_LRU_DIRTY = 0,
    __BFDEV_LRU_STARVING,

    BFDEV_LRU_DIRTY = BFDEV_BIT(__BFDEV_LRU_DIRTY),
    BFDEV_LRU_STARVING = BFDEV_BIT(__BFDEV_LRU_STARVING),
};

#define BFDEV_LRU_FREE_TAG UINT_MAX

/**
 * struct bfdev_lru_node - least recently used cache node.
 * @hash: for hash table lookup.
 * @list: lru list or free list.
 * @index: node index in cache head.
 */
struct bfdev_lru_node {
    struct bfdev_hlist_node hash;
    struct bfdev_list_head list;
    unsigned int index;

    unsigned int refcnt;
    unsigned int tag;
    bool uncommitted;
    void *pdata;
};

/**
 * struct bfdev_lru_head - least recently used cache head.
 * @lru: available node list headers.
 * @using: using node list.
 * @freed: released node list.
 * @changing: status transition node to be commit.
 * @maxpend: maximum number of simultaneous changes that can be pending.
 * @used: number of elements currently in use.
 * @pending: number of elements currently in transition.
 * @changed: time of state transitions.
 * @hits: time of cache hits.
 * @misses: time of cache misses.
 * @starve: time of cache starvation.
 */
struct bfdev_lru_head {
    const struct bfdev_alloc *alloc;
    struct bfdev_hlist_head *taghash;
    struct bfdev_lru_node **nodes;

    struct bfdev_list_head lru;
    struct bfdev_list_head using;
    struct bfdev_list_head freed;
    struct bfdev_list_head changing;

    /* const settings */
    unsigned int nmask;
    unsigned int maxpend;

    /* status description */
    unsigned long flags;
    unsigned int used;
    unsigned int pending;

    /* state counter */
    unsigned int changed;
    unsigned long hits;
    unsigned long misses;
    unsigned long starve;
};

BFDEV_BITFLAGS_STRUCT(lru, struct bfdev_lru_head, flags);
BFDEV_BITFLAGS_STRUCT_FLAG(lru, struct bfdev_lru_head, flags, dirty, __BFDEV_LRU_DIRTY);
BFDEV_BITFLAGS_STRUCT_FLAG(lru, struct bfdev_lru_head, flags, starving, __BFDEV_LRU_STARVING);

extern struct bfdev_lru_node *
bfdev_lru_find(struct bfdev_lru_head *head, unsigned int tag);

extern bool
bfdev_lru_check_used(struct bfdev_lru_head *head, unsigned int tag);

extern struct bfdev_lru_node *
bfdev_lru_obtain(struct bfdev_lru_head *head, unsigned int tag,
                 unsigned long flags);

extern unsigned int
bfdev_lru_put(struct bfdev_lru_head *head, struct bfdev_lru_node *node);

extern int
bfdev_lru_del(struct bfdev_lru_head *head, struct bfdev_lru_node *node);

extern void
bfdev_lru_set(struct bfdev_lru_head *head, struct bfdev_lru_node *node,
              unsigned int tag);

extern void
bfdev_lru_committed(struct bfdev_lru_head *head);

extern void
bfdev_lru_reset(struct bfdev_lru_head *head);

extern struct bfdev_lru_head *
bfdev_lru_create(const struct bfdev_alloc *alloc,
                 unsigned int size, unsigned int maxp);

extern void
bfdev_lru_destroy(struct bfdev_lru_head *head);

static inline struct bfdev_lru_node *
bfdev_lru_get(struct bfdev_lru_head *head, unsigned int tag)
{
    return bfdev_lru_obtain(head, tag, BFDEV_LRU_CHANGE);
}

static inline struct bfdev_lru_node *
bfdev_lru_try_get(struct bfdev_lru_head *head, unsigned int tag)
{
    return bfdev_lru_obtain(head, tag, 0);
}

static inline struct bfdev_lru_node *
bfdev_lru_cumulative(struct bfdev_lru_head *head, unsigned int tag)
{
    return bfdev_lru_obtain(head, tag, BFDEV_LRU_CHANGE | BFDEV_LRU_UNCOMMITTED);
}

BFDEV_END_DECLS

#endif /* _BFDEV_LRU_H_ */
