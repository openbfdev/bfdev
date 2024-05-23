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

typedef struct bfdev_cache_head bfdev_cache_head_t;
typedef struct bfdev_cache_node bfdev_cache_node_t;
typedef struct bfdev_cache_algo bfdev_cache_algo_t;

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
    bfdev_hlist_node_t hash;
    bfdev_list_head_t list;
    enum bfdev_cache_status status;

    unsigned long index;
    unsigned long tag;
    unsigned long refcnt;
    void *pdata;
};

struct bfdev_cache_head {
    const bfdev_alloc_t *alloc;
    const bfdev_cache_algo_t *algo;
    bfdev_hlist_head_t *taghash;
    bfdev_cache_node_t **nodes;

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

    bool (*starving)(bfdev_cache_head_t *head);
    bfdev_cache_node_t *(*obtain)(bfdev_cache_head_t *head);
    void (*get)(bfdev_cache_head_t *head, bfdev_cache_node_t *node);
    void (*put)(bfdev_cache_head_t *head, bfdev_cache_node_t *node);
    void (*update)(bfdev_cache_head_t *head, bfdev_cache_node_t *node);
    void (*clear)(bfdev_cache_head_t *head, bfdev_cache_node_t *node);
    void (*reset)(bfdev_cache_head_t *head);

    bfdev_cache_head_t *(*create)(const bfdev_alloc_t *alloc, unsigned long size);
    void (*destroy)(bfdev_cache_head_t *head);
};

BFDEV_BITFLAGS_STRUCT(
    bfdev_cache,
    bfdev_cache_head_t, flags
);

BFDEV_BITFLAGS_STRUCT_FLAG(
    bfdev_cache,
    bfdev_cache_head_t, flags,
    dirty, __BFDEV_CACHE_DIRTY
);

BFDEV_BITFLAGS_STRUCT_FLAG(
    bfdev_cache,
    bfdev_cache_head_t, flags,
    starving, __BFDEV_CACHE_STARVING
);

extern bfdev_cache_node_t *
bfdev_cache_find(bfdev_cache_head_t *head, unsigned long tag);

extern bfdev_cache_node_t *
bfdev_cache_obtain(bfdev_cache_head_t *head, unsigned long tag,
                   unsigned long flags);

extern unsigned long
bfdev_cache_put(bfdev_cache_head_t *head, bfdev_cache_node_t *node);

extern int
bfdev_cache_del(bfdev_cache_head_t *head, bfdev_cache_node_t *node);

extern int
bfdev_cache_set(bfdev_cache_head_t *head, bfdev_cache_node_t *node,
                unsigned long tag);

extern void
bfdev_cache_committed(bfdev_cache_head_t *head);

extern void
bfdev_cache_reset(bfdev_cache_head_t *head);

extern bfdev_cache_head_t *
bfdev_cache_create(const char *name, const bfdev_alloc_t *alloc,
                   unsigned long size, unsigned long maxp);

extern void
bfdev_cache_destroy(bfdev_cache_head_t *head);

static inline bfdev_cache_node_t *
bfdev_cache_get(bfdev_cache_head_t *head, unsigned long tag)
{
    return bfdev_cache_obtain(head, tag, BFDEV_CACHE_CHANGE);
}

static inline bfdev_cache_node_t *
bfdev_cache_try_get(bfdev_cache_head_t *head, unsigned long tag)
{
    return bfdev_cache_obtain(head, tag, 0);
}

static inline bfdev_cache_node_t *
bfdev_cache_cumulative(bfdev_cache_head_t *head, unsigned long tag)
{
    return bfdev_cache_obtain(head, tag, BFDEV_CACHE_CHANGE | BFDEV_CACHE_UNCOMMITTED);
}

extern int
bfdev_cache_register(bfdev_cache_algo_t *algo);

extern int
bfdev_cache_unregister(bfdev_cache_algo_t *algo);

BFDEV_END_DECLS

#endif /* _BFDEV_CACHE_H_ */
