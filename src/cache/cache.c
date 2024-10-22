/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log2.h>
#include <bfdev/hashtbl.h>
#include <bfdev/cache.h>
#include <export.h>

static
BFDEV_LIST_HEAD(cache_algorithms);

static bfdev_cache_algo_t *
cache_algorithm_find(const char *name)
{
    bfdev_cache_algo_t *walk;

    bfdev_list_for_each_entry(walk, &cache_algorithms, list) {
        if (!bfport_strcmp(walk->name, name))
            return walk;
    }

    return NULL;
}

static bool
cache_algorithm_exist(bfdev_cache_algo_t *algo)
{
    bfdev_cache_algo_t *walk;

    bfdev_list_for_each_entry(walk, &cache_algorithms, list) {
        if (walk == algo)
            return true;
    }

    return false;
}

static __bfdev_always_inline bool
cache_starving(bfdev_cache_head_t *head)
{
    return bfdev_list_check_empty(&head->freed) && head->algo->starving(head);
}

static __bfdev_always_inline unsigned long
cache_hash(bfdev_cache_head_t *head, const void *tag)
{
    const bfdev_cache_ops_t *ops;
    unsigned long hash;

    ops = head->ops;
    hash = ops->hash(tag, head->pdata);

    return hash;
}

static __bfdev_always_inline bool
cache_find(bfdev_cache_head_t *head, bfdev_cache_node_t *node, const char *tag)
{
    const bfdev_cache_ops_t *ops;
    unsigned long cmpval;

    ops = head->ops;
    cmpval = ops->find(node->data, tag, head->pdata);

    return !cmpval;
}

/* Find in changing, using or algos */
static bfdev_cache_node_t *
cache_lookup(bfdev_cache_head_t *head, const char *tag, bool change)
{
    bfdev_cache_node_t *walk;
    unsigned long hash, index;

    hash = cache_hash(head, tag);
    index = bfdev_hashtbl_index(head->size, hash);

    bfdev_hashtbl_for_each_idx_entry(walk, head->taghash, head->size, hash, index) {
        if (!cache_find(head, walk, tag))
            continue;

        if (walk->status != BFDEV_CACHE_PENDING || change)
            return walk;

        break;
    }

    return NULL;
}

/* Insert to from */
static void
cache_insert(bfdev_cache_head_t *head, bfdev_cache_node_t *node)
{
    unsigned long hash;

    hash = cache_hash(head, node->tag);
    bfdev_hashtbl_add(head->taghash, head->size, &node->hash, hash);
}

/* Obtain in freed or algos */
static bfdev_cache_node_t *
cache_obtain(bfdev_cache_head_t *head, const char *tag)
{
    const bfdev_cache_algo_t *algo;
    bfdev_cache_node_t *node;

    algo = head->algo;
    if (bfdev_list_check_empty(&head->freed)) {
        /* Get form algos */
        node = algo->obtain(head);
        bfdev_hashtbl_del(&node->hash);
    } else {
        /* Get form freed */
        node = bfdev_list_first_entry(
            &head->freed, bfdev_cache_node_t, list
        );
    }

    if (algo->clear)
        algo->clear(head, node);

    node->tag = tag;
    node->status = BFDEV_CACHE_PENDING;

    bfdev_list_move(&head->changing, &node->list);
    cache_insert(head, node);

    return node;
}

export bfdev_cache_node_t *
bfdev_cache_find(bfdev_cache_head_t *head, const void *tag)
{
    return cache_lookup(head, tag, false);
}

export bfdev_cache_node_t *
bfdev_cache_obtain(bfdev_cache_head_t *head, const void *tag, unsigned long flags)
{
    const bfdev_cache_algo_t *algo;
    bfdev_cache_node_t *node;

    if (bfdev_unlikely(bfdev_cache_starving_test(head))) {
        head->starve++;
        return NULL;
    }

    node = cache_lookup(head, tag, true);
    if (bfdev_likely(node)) {
        head->hits++;

        if (node->status == BFDEV_CACHE_PENDING) {
            if (!bfdev_cache_uncommitted_test(&flags))
                return NULL;

            node->refcnt++;
            return node;
        }

        algo = head->algo;
        if (node->status == BFDEV_CACHE_MANAGED)
            algo->get(head, node);

        if (algo->update)
            algo->update(head, node);

        if (!node->refcnt++)
            head->used++;

        bfdev_list_move(&head->using, &node->list);
        node->status = BFDEV_CACHE_USING;

        return node;
    }

    head->misses++;
    if (!bfdev_cache_change_test(&flags))
        return NULL;

    bfdev_cache_dirty_set(head);
    if (bfdev_unlikely(cache_starving(head))) {
        bfdev_cache_starving_set(head);
        return NULL;
    }

    if (bfdev_unlikely(head->pending >= head->maxpend))
        return NULL;

    node = cache_obtain(head, tag);
    BFDEV_BUG_ON(!node);

    bfdev_cache_starving_clr(head);
    node->refcnt++;
    head->pending++;
    head->used++;

    return node;
}

export unsigned long
bfdev_cache_put(bfdev_cache_head_t *head, bfdev_cache_node_t *node)
{
    if (bfdev_unlikely(node->status != BFDEV_CACHE_USING))
        return -BFDEV_EINVAL;

    if (!--node->refcnt) {
        bfdev_cache_starving_clr(head);
        bfdev_list_del_init(&node->list);

        head->algo->put(head, node);
        node->status = BFDEV_CACHE_MANAGED;
        head->used--;
    }

    return node->refcnt;
}

export int
bfdev_cache_set(bfdev_cache_head_t *head, bfdev_cache_node_t *node,
                const void *tag)
{
    const bfdev_cache_algo_t *algo;

    if (bfdev_unlikely(node->status != BFDEV_CACHE_MANAGED))
        return -BFDEV_EBUSY;

    algo = head->algo;
    node->tag = tag;

    algo->get(head, node);
    bfdev_hashtbl_del(&node->hash);
    cache_insert(head, node);

    if (algo->update)
        algo->update(head, node);
    algo->put(head, node);

    return -BFDEV_ENOERR;
}

export int
bfdev_cache_del(bfdev_cache_head_t *head, bfdev_cache_node_t *node)
{
    const bfdev_cache_algo_t *algo;

    if (bfdev_unlikely(node->status != BFDEV_CACHE_MANAGED))
        return -BFDEV_EBUSY;

    algo = head->algo;
    node->status = BFDEV_CACHE_FREED;

    algo->get(head, node);
    bfdev_hashtbl_del(&node->hash);
    bfdev_list_move(&head->freed, &node->list);

    return -BFDEV_ENOERR;
}

export void
bfdev_cache_committed(bfdev_cache_head_t *head)
{
    bfdev_cache_node_t *node, *tmp;

    bfdev_list_for_each_entry_safe(node, tmp, &head->changing, list) {
        bfdev_list_move(&head->using, &node->list);
        node->status = BFDEV_CACHE_USING;
        head->changed++;
    };

    head->pending = 0;
}

export void
bfdev_cache_reset(bfdev_cache_head_t *head)
{
    bfdev_cache_node_t *node;
    unsigned long count;

    head->flags = 0;
    head->pending = 0;
    head->used = 0;
    head->hits = 0;
    head->misses = 0;
    head->starve = 0;

    bfdev_list_head_init(&head->using);
    bfdev_list_head_init(&head->freed);
    bfdev_list_head_init(&head->changing);

    head->algo->reset(head);
    bfport_memset(head->taghash, 0, sizeof(*head->taghash) * head->size);

    for (count = 0; count < head->size; ++count) {
        node = head->nodes[count];
        bfdev_list_add(&head->freed, &node->list);
    }
}

export bfdev_cache_head_t *
bfdev_cache_create(const char *name, const bfdev_alloc_t *alloc,
                   const bfdev_cache_ops_t *ops, unsigned long size,
                   unsigned long maxpend, void *pdata)
{
    const bfdev_cache_algo_t *algo;
    bfdev_cache_head_t *head;
    unsigned long count;

    if (!ops->hash || !ops->find)
        return NULL;

    size = bfdev_pow2_roundup(size);
    if (bfdev_unlikely(size < 2))
        return NULL;

    algo = cache_algorithm_find(name);
    if (bfdev_unlikely(!algo))
        return NULL;

    head = algo->create(alloc, size);
    if (bfdev_unlikely(!head))
        return NULL;

    head->algo = algo;
    head->alloc = alloc;
    head->size = size;
    head->maxpend = maxpend;

    head->ops = ops;
    head->pdata = pdata;

    head->taghash = bfdev_zalloc_array(alloc, size, sizeof(*head->taghash));
    if (bfdev_unlikely(!head->taghash)) {
        algo->destroy(head);
        return NULL;
    }

    bfdev_list_head_init(&head->using);
    bfdev_list_head_init(&head->freed);
    bfdev_list_head_init(&head->changing);

    for (count = 0; count < size; ++count) {
        bfdev_cache_node_t *node;

        node = head->nodes[count];
        node->index = count;
        bfdev_list_add(&head->freed, &node->list);
    }

    return head;
}

export void
bfdev_cache_destroy(bfdev_cache_head_t *head)
{
    const bfdev_cache_algo_t *algo;
    const bfdev_alloc_t *alloc;

    algo = head->algo;
    alloc = head->alloc;

    bfdev_free(alloc, head->taghash);
    algo->destroy(head);
}

export int
bfdev_cache_register(bfdev_cache_algo_t *algo)
{
    if (!(algo->name && algo->create && algo->destroy &&
          algo->reset && algo->starving && algo->obtain &&
          algo->get && algo->put))
        return -BFDEV_EINVAL;

    if (cache_algorithm_find(algo->name))
        return -BFDEV_EALREADY;

    bfdev_list_add(&cache_algorithms, &algo->list);

    return -BFDEV_ENOERR;
}

export int
bfdev_cache_unregister(bfdev_cache_algo_t *algo)
{
    if (!cache_algorithm_exist(algo))
        return -BFDEV_ENOENT;

    bfdev_list_del(&algo->list);

    return -BFDEV_ENOERR;
}
