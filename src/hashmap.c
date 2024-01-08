/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/hashmap.h>
#include <export.h>

static __bfdev_always_inline unsigned long
hashmap_hash_node(bfdev_hashmap_t *hashmap,
                  const struct bfdev_hlist_node *node)
{
    const bfdev_hashmap_ops_t *ops;
    unsigned long retval;

    ops = hashmap->ops;
    retval = ops->hash_node(node, hashmap->pdata);

    return retval;
}

static __bfdev_always_inline unsigned long
hashmap_hash_key(const bfdev_hashmap_t *hashmap,
                 const void *key)
{
    const bfdev_hashmap_ops_t *ops;
    unsigned long retval;

    ops = hashmap->ops;
    retval = ops->hash_key(key, hashmap->pdata);

    return retval;
}

static __bfdev_always_inline long
hashmap_equal(bfdev_hashmap_t *hashmap, const struct bfdev_hlist_node *node1,
              const struct bfdev_hlist_node *node2)
{
    const bfdev_hashmap_ops_t *ops;
    long retval;

    ops = hashmap->ops;
    retval = ops->equal(node1, node2, hashmap->pdata);

    return retval;
}

static __bfdev_always_inline long
hashmap_find(bfdev_hashmap_t *hashmap, const void *key,
             const struct bfdev_hlist_node *node)
{
    const bfdev_hashmap_ops_t *ops;
    long retval;

    ops = hashmap->ops;
    retval = ops->find(node, key, hashmap->pdata);

    return retval;
}

static __bfdev_always_inline bool
hashmap_need_extend(bfdev_hashmap_t *hashmap)
{
    const bfdev_hashmap_ops_t *ops;

    if (!hashmap->capacity)
        return true;

    ops = hashmap->ops;
    if (ops->extend)
        return ops->extend(hashmap, hashmap->pdata);

    /* Extend if empty or more than 75% filled */
    return hashmap->used * 4 / 3 > hashmap->capacity;
}

static __bfdev_always_inline bool
hashmap_need_shrink(bfdev_hashmap_t *hashmap)
{
    const bfdev_hashmap_ops_t *ops;

    ops = hashmap->ops;
    if (ops->shrink)
        return ops->shrink(hashmap, hashmap->pdata);

    /* Shrink if less than 25% filled */
    return hashmap->used * 4 < hashmap->capacity;
}

static inline struct bfdev_hlist_node *
hashmap_find_node(bfdev_hashmap_t *hashmap, const struct bfdev_hlist_node *node,
                  unsigned long hash)
{
    struct bfdev_hlist_node *walk;
    unsigned long index;

    if (!hashmap->buckets)
        return NULL;

    index = bfdev_hashtbl_index(hashmap->capacity, hash);
    bfdev_hashtbl_for_each_idx(walk, hashmap->buckets, hashmap->capacity, index) {
        if (!hashmap_equal(hashmap, node, walk))
            return walk;
    }

    return NULL;
}

static inline struct bfdev_hlist_node *
hashmap_find_key(bfdev_hashmap_t *hashmap, const void *key,
                 unsigned long hash)
{
    struct bfdev_hlist_node *walk;
    unsigned long index;

    if (!hashmap->buckets)
        return NULL;

    index = bfdev_hashtbl_index(hashmap->capacity, hash);
    bfdev_hashtbl_for_each_idx(walk, hashmap->buckets, hashmap->capacity, index) {
        if (!hashmap_find(hashmap, key, walk))
            return walk;
    }

    return NULL;
}

static inline int
hashmap_rehash(bfdev_hashmap_t *hashmap, unsigned int nbits)
{
    const struct bfdev_alloc *alloc = hashmap->alloc;
    struct bfdev_hlist_node *walk, *tmp;
    struct bfdev_hlist_head *nbuckets;
    unsigned long value, index, ncapacity;

    ncapacity = BFDEV_BIT(nbits);
    nbuckets = bfdev_malloc_array(alloc, ncapacity, sizeof(*nbuckets));
    if (!nbuckets)
        return -BFDEV_ENOMEM;

    bfdev_hashtbl_init(nbuckets, ncapacity);
    bfdev_hashtbl_for_each_safe(walk, tmp, hashmap->buckets, hashmap->capacity, index) {
        value = hashmap_hash_node(hashmap, walk);
        bfdev_hlist_del(walk);
        bfdev_hashtbl_add(nbuckets, ncapacity, walk, value);
    }

    bfdev_free(alloc, hashmap->buckets);

    hashmap->capacity = ncapacity;
    hashmap->bits = nbits;
    hashmap->buckets = nbuckets;

    return -BFDEV_ENOERR;
}

static inline int
hashmap_extend(bfdev_hashmap_t *hashmap)
{
    unsigned int nbits;

    nbits = hashmap->bits + 1;
    if (nbits < BFDEV_HASHMAP_MIN_BITS)
        nbits = BFDEV_HASHMAP_MIN_BITS;

    return hashmap_rehash(hashmap, nbits);
}

static inline int
hashmap_shrink(bfdev_hashmap_t *hashmap)
{
    unsigned int nbits;

    nbits = hashmap->bits - 1;
    if (nbits < BFDEV_HASHMAP_MIN_BITS)
        nbits = BFDEV_HASHMAP_MIN_BITS;

    return hashmap_rehash(hashmap, nbits);
}

export int
bfdev_hashmap_insert(bfdev_hashmap_t *hashmap, struct bfdev_hlist_node *node,
                     struct bfdev_hlist_node **old, enum bfdev_hashmap_strategy strategy)
{
    struct bfdev_hlist_node *exist;
    unsigned long value;
    int retval;

    value = hashmap_hash_node(hashmap, node);
    if (strategy != BFDEV_HASHMAP_APPEND &&
        (exist = hashmap_find_node(hashmap, node, value))) {
        if (old)
            *old = exist;

        if (strategy == BFDEV_HASHMAP_ADD)
            return -BFDEV_EEXIST;

        /* BFDEV_HASHMAP_{SET / UPDATE} */
        bfdev_hlist_replace(exist, node);
        return -BFDEV_ENOERR;
    }

    if (strategy == BFDEV_HASHMAP_UPDATE)
        return -BFDEV_ENOENT;

    if (hashmap_need_extend(hashmap)) {
        retval = hashmap_extend(hashmap);
        if (retval)
            return retval;
        value = hashmap_hash_node(hashmap, node);
    }

    bfdev_hashtbl_add(hashmap->buckets, hashmap->capacity, node, value);
    hashmap->used++;

    return -BFDEV_ENOERR;
}

export int
bfdev_hashmap_del(bfdev_hashmap_t *hashmap, const void *key,
                  struct bfdev_hlist_node **node)
{
    struct bfdev_hlist_node *exist;
    unsigned long value;

    value = hashmap_hash_key(hashmap, key);
    if (!(exist = hashmap_find_key(hashmap, key, value)))
        return -BFDEV_ENOENT;

    bfdev_hashtbl_del(exist);
    hashmap->used--;

    if (hashmap_need_shrink(hashmap))
        hashmap_shrink(hashmap);

    if (node)
        *node = exist;

    return -BFDEV_ENOERR;
}

export struct bfdev_hlist_node *
bfdev_hashmap_find(bfdev_hashmap_t *hashmap, const void *key)
{
    struct bfdev_hlist_node *exist;
    unsigned long value;

    value = hashmap_hash_key(hashmap, key);
    if ((exist = hashmap_find_key(hashmap, key, value)))
        return exist;

    return NULL;
}

export void
bfdev_hashmap_release(bfdev_hashmap_t *hashmap)
{
    const struct bfdev_alloc *alloc = hashmap->alloc;

    bfdev_free(alloc, hashmap->buckets);
    hashmap->buckets = NULL;

    hashmap->used = 0;
    hashmap->capacity = 0;
    hashmap->bits = 0;
}
