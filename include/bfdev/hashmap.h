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

typedef struct bfdev_hashmap bfdev_hashmap_t;
typedef struct bfdev_hashmap_ops bfdev_hashmap_ops_t;

/**
 * enum bfdev_hashmap_strategy - Hashmap insertion strategy.
 * @HASHMAP_ADD: only add key/value if key doesn't exist yet.
 * @HASHMAP_SET: add key/value pair if key doesn't exist yet. otherwise update value;
 * @HASHMAP_UPDATE: only update value, if key already exists.
 * @HASHMAP_APPEND: always add key/value pair, even if key already exists.
 */
enum bfdev_hashmap_strategy {
    BFDEV_HASHMAP_ADD = 0,
    BFDEV_HASHMAP_SET,
    BFDEV_HASHMAP_UPDATE,
    BFDEV_HASHMAP_APPEND,
};

struct bfdev_hashmap {
    bfdev_hlist_head_t *buckets;
    unsigned int bits;
    unsigned long capacity;
    unsigned long used;

    const bfdev_alloc_t *alloc;
    const bfdev_hashmap_ops_t *ops;
    void *pdata;
};

struct bfdev_hashmap_ops {
    unsigned long (*hash_key)(const void *key, void *pdata);
    unsigned long (*hash_node)(const bfdev_hlist_node_t *node, void *pdata);
    long (*equal)(const bfdev_hlist_node_t *node1, const bfdev_hlist_node_t *node2, void *pdata);
    long (*find)(const bfdev_hlist_node_t *node, const void *key, void *pdata);

    bool (*extend)(const bfdev_hashmap_t *hashmap, void *pdata);
    bool (*shrink)(const bfdev_hashmap_t *hashmap, void *pdata);
};

#define BFDEV_HASHMAP_STATIC(ALLOC, OPS, PDATA) { \
    .alloc = (ALLOC), .ops = (OPS), .pdata = (PDATA), \
}

#define BFDEV_HASHMAP_INIT(alloc, ops, pdata) \
    (bfdev_hashmap_t) BFDEV_HASHMAP_STATIC(alloc, ops, pdata)

#define BFDEV_DEFINE_HASHMAP(name, alloc, ops, pdata) \
    bfdev_hashmap_t name = BFDEV_HASHMAP_INIT(alloc, ops, pdata)

/**
 * bfdev_hashmap_init() - initialize a hashmap structure.
 * @hashmap: hashmap structure to be initialized.
 * @alloc: allocator operations.
 * @ops: hashmap operations.
 * @pdata: operations callback data.
 */
static inline void
bfdev_hashmap_init(bfdev_hashmap_t *hashmap, const bfdev_alloc_t *alloc,
                   const bfdev_hashmap_ops_t *ops, void *pdata)
{
    *hashmap = BFDEV_HASHMAP_INIT(alloc, ops, pdata);
}

/**
 * bfdev_hashmap_insert() - insert a hashlist node to hashmap.
 * @hashmap: hashmap structure to be insert.
 * @node: new hashlist node to insert.
 * @old: pointer used to return the replaced node.
 * @strategy: insertion strategy.
 */
extern int
bfdev_hashmap_insert(bfdev_hashmap_t *hashmap, bfdev_hlist_node_t *node,
                     bfdev_hlist_node_t **old, enum bfdev_hashmap_strategy strategy);

/**
 * bfdev_hashmap_del() - delete a hashlist node from hashmap.
 * @hashmap: hashmap structure to be delete.
 * @key: key of the node to be deleted.
 * @node: pointer used to return the deleted node.
 */
extern int
bfdev_hashmap_del(bfdev_hashmap_t *hashmap, const void *key,
                  bfdev_hlist_node_t **node);

/**
 * bfdev_hashmap_find() - find a hashlist node in hashmap.
 * @hashmap: hashmap structure to be find.
 * @key: key of the node to be find.
 */
extern bfdev_hlist_node_t *
bfdev_hashmap_find(bfdev_hashmap_t *hashmap, const void *key);

/**
 * bfdev_hashmap_release() - release hash bucket in hashmap.
 * @hashmap: hashmap structure to be release.
 */
extern void
bfdev_hashmap_release(bfdev_hashmap_t *hashmap);

static __bfdev_always_inline int
bfdev_hashmap_add(bfdev_hashmap_t *hashmap, bfdev_hlist_node_t *node)
{
    return bfdev_hashmap_insert(hashmap, node, NULL, BFDEV_HASHMAP_ADD);
}

static __bfdev_always_inline int
bfdev_hashmap_set(bfdev_hashmap_t *hashmap, bfdev_hlist_node_t *node,
                  bfdev_hlist_node_t **old)
{
    return bfdev_hashmap_insert(hashmap, node, old, BFDEV_HASHMAP_SET);
}

static __bfdev_always_inline int
bfdev_hashmap_update(bfdev_hashmap_t *hashmap, bfdev_hlist_node_t *node,
                     bfdev_hlist_node_t **old)
{
    return bfdev_hashmap_insert(hashmap, node, old, BFDEV_HASHMAP_UPDATE);
}

static __bfdev_always_inline int
bfdev_hashmap_append(bfdev_hashmap_t *hashmap, bfdev_hlist_node_t *node)
{
    return bfdev_hashmap_insert(hashmap, node, NULL, BFDEV_HASHMAP_APPEND);
}

BFDEV_END_DECLS

#endif /* _BFDEV_HASHMAP_H_ */
