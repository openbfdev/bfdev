/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CACHE_H_
#define _BFDEV_CACHE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/allocator.h>
#include <bfdev/list.h>
#include <bfdev/hlist.h>
#include <bfdev/bitflags.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_CACHE_FREE_TAG
# define BFDEV_CACHE_FREE_TAG ULONG_MAX
#endif

enum bfdev_cache_obtain {
    __BFDEV_CACHE_CHANGE = 0,
    __BFDEV_CACHE_UNCOMMITTED,

    BFDEV_CACHE_CHANGE = BFDEV_BIT(__BFDEV_CACHE_CHANGE),
    BFDEV_CACHE_UNCOMMITTED = BFDEV_BIT(__BFDEV_CACHE_UNCOMMITTED),
};

enum bfdev_cache_flags {
    __BFDEV_CACHE_DIRTY = 0,
    __BFDEV_CACHE_STARVING,

    BFDEV_CACHE_DIRTY = BFDEV_BIT(__BFDEV_CACHE_DIRTY),
    BFDEV_CACHE_STARVING = BFDEV_BIT(__BFDEV_CACHE_STARVING),
};

enum bfdev_cache_status {
    BFDEV_CACHE_FREED = 0,
    BFDEV_CACHE_PENDING,
    BFDEV_CACHE_USING,
    BFDEV_CACHE_MANAGED,
};

struct bfdev_cache_node {
    struct bfdev_hlist_node hash;
    bfdev_list_head_t list;
    enum bfdev_cache_status status;

    unsigned long index;
    unsigned long tag;
    unsigned long refcnt;
    void *pdata;
};

struct bfdev_cache_head {
    const struct bfdev_alloc *alloc;
    const struct bfdev_cache_algo *algo;
    struct bfdev_hlist_head *taghash;
    struct bfdev_cache_node **nodes;

    bfdev_list_head_t using;
    bfdev_list_head_t freed;
    bfdev_list_head_t changing;

    /* const settings */
    unsigned long size;
    unsigned long maxpend;

    /* status description */
    unsigned long flags;
    unsigned long used;
    unsigned long pending;

    /* state counter */
    unsigned long changed;
    unsigned long starve;
    unsigned long hits;
    unsigned long misses;
};

struct bfdev_cache_algo {
    bfdev_list_head_t list;
    const char *name;

    bool (*starving)(struct bfdev_cache_head *head);
    struct bfdev_cache_node *(*obtain)(struct bfdev_cache_head *head);
    void (*get)(struct bfdev_cache_head *head, struct bfdev_cache_node *node);
    void (*put)(struct bfdev_cache_head *head, struct bfdev_cache_node *node);
    void (*update)(struct bfdev_cache_head *head, struct bfdev_cache_node *node);
    void (*clear)(struct bfdev_cache_head *head, struct bfdev_cache_node *node);
    void (*reset)(struct bfdev_cache_head *head);

    struct bfdev_cache_head *(*create)(const struct bfdev_alloc *alloc, unsigned long size);
    void (*destroy)(struct bfdev_cache_head *head);
};

BFDEV_BITFLAGS_STRUCT(bfdev_cache,
    struct bfdev_cache_head, flags
);

BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_cache,
    struct bfdev_cache_head, flags,
    dirty, __BFDEV_CACHE_DIRTY
);

BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_cache,
    struct bfdev_cache_head, flags,
    starving, __BFDEV_CACHE_STARVING
);

extern struct bfdev_cache_node *
bfdev_cache_find(struct bfdev_cache_head *head, unsigned long tag);

extern struct bfdev_cache_node *
bfdev_cache_obtain(struct bfdev_cache_head *head, unsigned long tag,
                   unsigned long flags);

extern unsigned long
bfdev_cache_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node);

extern int
bfdev_cache_del(struct bfdev_cache_head *head, struct bfdev_cache_node *node);

extern int
bfdev_cache_set(struct bfdev_cache_head *head, struct bfdev_cache_node *node,
                unsigned long tag);

extern void
bfdev_cache_committed(struct bfdev_cache_head *head);

extern void
bfdev_cache_reset(struct bfdev_cache_head *head);

extern struct bfdev_cache_head *
bfdev_cache_create(const char *name, const struct bfdev_alloc *alloc,
                   unsigned long size, unsigned long maxp);

extern void
bfdev_cache_destroy(struct bfdev_cache_head *head);

static inline struct bfdev_cache_node *
bfdev_cache_get(struct bfdev_cache_head *head, unsigned long tag)
{
    return bfdev_cache_obtain(head, tag, BFDEV_CACHE_CHANGE);
}

static inline struct bfdev_cache_node *
bfdev_cache_try_get(struct bfdev_cache_head *head, unsigned long tag)
{
    return bfdev_cache_obtain(head, tag, 0);
}

static inline struct bfdev_cache_node *
bfdev_cache_cumulative(struct bfdev_cache_head *head, unsigned long tag)
{
    return bfdev_cache_obtain(head, tag, BFDEV_CACHE_CHANGE | BFDEV_CACHE_UNCOMMITTED);
}

extern int
bfdev_cache_register(struct bfdev_cache_algo *algo);

extern void
bfdev_cache_unregister(struct bfdev_cache_algo *algo);

BFDEV_END_DECLS

#endif /* _BFDEV_CACHE_H_ */
