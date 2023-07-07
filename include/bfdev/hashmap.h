/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HASHMAP_H_
#define _BFDEV_HASHMAP_H_

#include <bfdev/config.h>
#include <bfdev/errno.h>
#include <bfdev/hashtbl.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_HASHMAP_MIN_BITS
# define BFDEV_HASHMAP_MIN_BITS 4
#endif

/**
 * enum bfdev_hashmap_strategy - Hashmap insertion strategy.
 * @HASHMAP_ADD: only add key/value if key doesn't exist yet.
 * @HASHMAP_SET: add key/value pair if key doesn't exist yet.
 * @HASHMAP_UPDATE: update value, if key already exists.
 * @HASHMAP_APPEND: always add key/value pair, even if key already exists.
 */
enum bfdev_hashmap_strategy {
    BFDEV_HASHMAP_ADD = 0,
    BFDEV_HASHMAP_SET,
    BFDEV_HASHMAP_UPDATE,
    BFDEV_HASHMAP_APPEND,
};

struct bfdev_hashmap {
    struct bfdev_hlist_head *buckets;
    unsigned int bits;
    unsigned long capacity;
    unsigned long used;

    const struct bfdev_alloc *alloc;
    const struct bfdev_hashmap_ops *ops;
    void *pdata;
};

struct bfdev_hashmap_ops {
    unsigned long (*hash_key)(const void *key, void *pdata);
    unsigned long (*hash_node)(const struct bfdev_hlist_node *node, void *pdata);
    long (*equal)(const struct bfdev_hlist_node *nodea, const struct bfdev_hlist_node *nodeb, void *pdata);
    long (*find)(const struct bfdev_hlist_node *node, const void *key, void *pdata);

    bool (*extend)(const struct bfdev_hashmap *hashmap, void *pdata);
    bool (*shrink)(const struct bfdev_hashmap *hashmap, void *pdata);
};

#define BFDEV_HASHMAP_STATIC(ALLOC, OPS, PDATA) {       \
    .alloc = (ALLOC), .ops = (OPS), .pdata = (PDATA),   \
}

#define BFDEV_HASHMAP_INIT(alloc, ops, pdata) \
    (struct bfdev_hashmap) BFDEV_HASHMAP_STATIC(alloc, ops, pdata)

#define BFDEV_DEFINE_HASHMAP(name, alloc, ops, pdata) \
    struct bfdev_hashmap name = BFDEV_HASHMAP_INIT(alloc, ops, pdata)

static inline void
bfdev_hashmap_init(struct bfdev_hashmap *hashmap, const struct bfdev_alloc *alloc,
                   const struct bfdev_hashmap_ops *ops, void *pdata)
{
    *hashmap = BFDEV_HASHMAP_INIT(alloc, ops, pdata);
}

extern int
bfdev_hashmap_insert(struct bfdev_hashmap *hashmap, struct bfdev_hlist_node *node,
                     struct bfdev_hlist_node **old, enum bfdev_hashmap_strategy strategy);

extern int
bfdev_hashmap_del(struct bfdev_hashmap *hashmap, const void *key,
                  struct bfdev_hlist_node **node);

extern struct bfdev_hlist_node *
bfdev_hashmap_find(struct bfdev_hashmap *hashmap, const void *key);

extern void
bfdev_hashmap_release(struct bfdev_hashmap *hashmap);

static __bfdev_always_inline int
bfdev_hashmap_add(struct bfdev_hashmap *hashmap, struct bfdev_hlist_node *node)
{
    return bfdev_hashmap_insert(hashmap, node, NULL, BFDEV_HASHMAP_ADD);
}

static __bfdev_always_inline int
bfdev_hashmap_set(struct bfdev_hashmap *hashmap, struct bfdev_hlist_node *node,
                  struct bfdev_hlist_node **old)
{
    return bfdev_hashmap_insert(hashmap, node, old, BFDEV_HASHMAP_SET);
}

static __bfdev_always_inline int
bfdev_hashmap_update(struct bfdev_hashmap *hashmap, struct bfdev_hlist_node *node,
                     struct bfdev_hlist_node **old)
{
    return bfdev_hashmap_insert(hashmap, node, old, BFDEV_HASHMAP_UPDATE);
}

static __bfdev_always_inline int
bfdev_hashmap_append(struct bfdev_hashmap *hashmap, struct bfdev_hlist_node *node)
{
    return bfdev_hashmap_insert(hashmap, node, NULL, BFDEV_HASHMAP_APPEND);
}

BFDEV_END_DECLS

#endif /* _BFDEV_HASHMAP_H_ */
