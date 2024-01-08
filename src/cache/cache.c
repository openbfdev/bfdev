/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log2.h>
#include <bfdev/hashtbl.h>
#include <bfdev/cache.h>
#include <export.h>

static BFDEV_LIST_HEAD(cache_algorithms);

static struct bfdev_cache_algo *
cache_algorithm_find(const char *name)
{
    struct bfdev_cache_algo *algo;

    bfdev_list_for_each_entry(algo, &cache_algorithms, list) {
        if (!strcmp(algo->name, name))
            return algo;
    }

    return NULL;
}

static __bfdev_always_inline bool
cache_starving(struct bfdev_cache_head *head)
{
    return bfdev_list_check_empty(&head->freed) &&
           head->algo->starving(head);
}

/* Find in changing, using or algos */
static struct bfdev_cache_node *
cache_find(struct bfdev_cache_head *head, unsigned long tag, bool change)
{
    struct bfdev_cache_node *walk;
    unsigned long index;

    index = bfdev_hashtbl_index(head->size, tag);
    bfdev_hashtbl_for_each_idx_entry(walk, head->taghash, head->size, hash, index) {
        if (walk->tag != tag)
            continue;
        if (walk->status != BFDEV_CACHE_PENDING || change)
            return walk;
        break;
    }

    return NULL;
}

/* Obtain in freed or algos */
static struct bfdev_cache_node *
cache_obtain(struct bfdev_cache_head *head, unsigned long tag)
{
    const struct bfdev_cache_algo *algo;
    struct bfdev_cache_node *node;

    algo = head->algo;
    if (bfdev_list_check_empty(&head->freed)) {
        /* Get form algos */
        node = algo->obtain(head);
        bfdev_hashtbl_del(&node->hash);
    } else {
        /* Get form freed */
        node = bfdev_list_first_entry(
            &head->freed, struct bfdev_cache_node, list
        );
    }

    if (algo->clear)
        algo->clear(head, node);

    node->tag = tag;
    node->status = BFDEV_CACHE_PENDING;

    bfdev_list_move(&head->changing, &node->list);
    bfdev_hashtbl_add(head->taghash, head->size, &node->hash, tag);

    return node;
}

export struct bfdev_cache_node *
bfdev_cache_find(struct bfdev_cache_head *head, unsigned long tag)
{
    return cache_find(head, tag, false);
}

export struct bfdev_cache_node *
bfdev_cache_obtain(struct bfdev_cache_head *head, unsigned long tag,
                   unsigned long flags)
{
    const struct bfdev_cache_algo *algo;
    struct bfdev_cache_node *node;

    if (bfdev_unlikely(tag == BFDEV_CACHE_FREE_TAG))
        return NULL;

    if (bfdev_unlikely(bfdev_cache_test_starving(head))) {
        head->starve++;
        return NULL;
    }

    node = cache_find(head, tag, true);
    if (bfdev_likely(node)) {
        head->hits++;

        if (node->status == BFDEV_CACHE_PENDING) {
            if (!(flags & BFDEV_CACHE_UNCOMMITTED))
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
    if (!(flags & BFDEV_CACHE_CHANGE))
        return NULL;

    bfdev_cache_set_dirty(head);
    if (bfdev_unlikely(cache_starving(head))) {
        bfdev_cache_set_starving(head);
        return NULL;
    }

    if (bfdev_unlikely(head->pending >= head->maxpend))
        return NULL;

    node = cache_obtain(head, tag);
    bfdev_cache_clr_starving(head);

    node->refcnt++;
    head->pending++;
    head->used++;

    return node;
}

export unsigned long
bfdev_cache_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    if (bfdev_unlikely(node->status != BFDEV_CACHE_USING))
        return -BFDEV_EINVAL;

    if (!--node->refcnt) {
        bfdev_cache_clr_starving(head);
        bfdev_list_del_init(&node->list);

        head->algo->put(head, node);
        node->status = BFDEV_CACHE_MANAGED;
        head->used--;
    }

    return node->refcnt;
}

export int
bfdev_cache_set(struct bfdev_cache_head *head, struct bfdev_cache_node *node,
                unsigned long tag)
{
    const struct bfdev_cache_algo *algo;

    if (bfdev_unlikely(tag == BFDEV_CACHE_FREE_TAG))
        return -BFDEV_EINVAL;

    if (bfdev_unlikely(node->status != BFDEV_CACHE_MANAGED))
        return -BFDEV_EBUSY;

    algo = head->algo;
    node->tag = tag;

    algo->get(head, node);
    bfdev_hashtbl_del(&node->hash);
    bfdev_hashtbl_add(head->taghash, head->size, &node->hash, tag);

    if (algo->update)
        algo->update(head, node);
    algo->put(head, node);

    return -BFDEV_ENOERR;
}

export int
bfdev_cache_del(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    const struct bfdev_cache_algo *algo;

    if (bfdev_unlikely(node->status != BFDEV_CACHE_MANAGED))
        return -BFDEV_EBUSY;

    algo = head->algo;
    node->status = BFDEV_CACHE_FREED;
    node->tag = BFDEV_CACHE_FREE_TAG;

    algo->get(head, node);
    bfdev_hashtbl_del(&node->hash);
    bfdev_list_move(&head->freed, &node->list);

    return -BFDEV_ENOERR;
}

export void
bfdev_cache_committed(struct bfdev_cache_head *head)
{
    struct bfdev_cache_node *node, *tmp;

    bfdev_list_for_each_entry_safe(node, tmp, &head->changing, list) {
        bfdev_list_move(&head->using, &node->list);
        node->status = BFDEV_CACHE_USING;
        head->changed++;
    };

    head->pending = 0;
}

export void
bfdev_cache_reset(struct bfdev_cache_head *head)
{
    struct bfdev_cache_node *node;
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
    memset(head->taghash, 0, sizeof(*head->taghash) * head->size);

    for (count = 0; count < head->size; ++count) {
        node = head->nodes[count];
        memset(node, 0, sizeof(*node));

        node->index = count;
        node->tag = BFDEV_CACHE_FREE_TAG;
        bfdev_list_add(&head->freed, &node->list);
    }
}

export struct bfdev_cache_head *
bfdev_cache_create(const char *name, const bfdev_alloc_t *alloc,
                   unsigned long size, unsigned long maxpend)
{
    const struct bfdev_cache_algo *algo;
    struct bfdev_cache_head *head;
    unsigned long count;

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

    head->taghash = bfdev_zalloc_array(alloc, size, sizeof(*head->taghash));
    if (bfdev_unlikely(!head->taghash)) {
        algo->destroy(head);
        return NULL;
    }

    bfdev_list_head_init(&head->using);
    bfdev_list_head_init(&head->freed);
    bfdev_list_head_init(&head->changing);

    for (count = 0; count < size; ++count) {
        struct bfdev_cache_node *node;

        node = head->nodes[count];
        node->index = count;
        node->tag = BFDEV_CACHE_FREE_TAG;
        bfdev_list_add(&head->freed, &node->list);
    }

    return head;
}

export void
bfdev_cache_destroy(struct bfdev_cache_head *head)
{
    const struct bfdev_cache_algo *algo;
    const bfdev_alloc_t *alloc;

    algo = head->algo;
    alloc = head->alloc;

    bfdev_free(alloc, head->taghash);
    algo->destroy(head);
}

export int
bfdev_cache_register(struct bfdev_cache_algo *algo)
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

export void
bfdev_cache_unregister(struct bfdev_cache_algo *algo)
{
    if (cache_algorithm_find(algo->name))
        return;

    bfdev_list_del(&algo->list);
}
