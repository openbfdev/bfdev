/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log2.h>
#include <bfdev/hashtbl.h>
#include <bfdev/cache.h>
#include <export.h>

static BFDEV_LIST_HEAD(algorithms);

static struct bfdev_cache_algo *
algorithm_find(const char *name)
{
    struct bfdev_cache_algo *algo;

    bfdev_list_for_each_entry(algo, &algorithms, list) {
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

static struct bfdev_cache_node *
cache_find(struct bfdev_cache_head *head, unsigned long tag, bool change)
{
    struct bfdev_cache_node *walk;
    unsigned long index;

    index = bfdev_hashtbl_index(head->size, tag);
    bfdev_hashtbl_for_each_idx_entry(walk, head->taghash, head->size, hash, index) {
        if (walk->tag != tag)
            continue;
        if (!walk->uncommitted || change)
            return walk;
        break;
    }

    return NULL;
}

static struct bfdev_cache_node *
cache_obtain(struct bfdev_cache_head *head, unsigned long tag)
{
    struct bfdev_cache_node *node;

    if (bfdev_list_check_empty(&head->freed))
        node = head->algo->obtain(head);
    else {
        node = bfdev_list_first_entry(
            &head->freed, struct bfdev_cache_node, list
        );
    }

    if (!bfdev_hlist_check_unhashed(&node->hash))
        bfdev_hlist_del(&node->hash);

    node->tag = tag;
    node->uncommitted = true;

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
    struct bfdev_cache_node *node;

    if (bfdev_unlikely(bfdev_cache_test_starving(head))) {
        head->starve++;
        return NULL;
    }

    node = cache_find(head, tag, true);
    if (node) {
        head->hits++;
        if (node->uncommitted) {
            if (!(flags & BFDEV_CACHE_UNCOMMITTED))
                return NULL;
            node->refcnt++;
        } else {
            if (!node->refcnt++)
                head->used++;
            bfdev_list_move(&head->using, &node->list);
        }

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

    head->pending++;
    head->used++;

    return node;
}

export unsigned long
bfdev_cache_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    if (!--node->refcnt) {
        bfdev_cache_clr_starving(head);
        head->algo->put(head, node);
        head->used--;
    }

    return node->refcnt;
}

export int
bfdev_cache_set(struct bfdev_cache_head *head, struct bfdev_cache_node *node,
                unsigned long tag)
{
    if (node->refcnt)
        return -BFDEV_EBUSY;

    node->tag = tag;
    if (!bfdev_hlist_check_unhashed(&node->hash))
        bfdev_hlist_del(&node->hash);

    if (tag == BFDEV_CACHE_FREE_TAG) {
        bfdev_hlist_node_init(&node->hash);
        bfdev_list_move(&head->freed, &node->list);
    } else {
        bfdev_hashtbl_add(head->taghash, head->size, &node->hash, tag);
        head->algo->put(head, node);
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_cache_del(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    if (node->refcnt)
        return -BFDEV_EBUSY;

    node->tag = BFDEV_CACHE_FREE_TAG;
    bfdev_hlist_del_init(&node->hash);
    bfdev_list_move(&head->freed, &node->list);

    return -BFDEV_ENOERR;
}

export void
bfdev_cache_committed(struct bfdev_cache_head *head)
{
    struct bfdev_cache_node *node, *tmp;

    bfdev_list_for_each_entry_safe(node, tmp, &head->changing, list) {
        bfdev_list_move(&head->using, &node->list);
        node->uncommitted = false;
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
bfdev_cache_create(const char *name, const struct bfdev_alloc *alloc,
                   unsigned long size, unsigned long maxpend)
{
    const struct bfdev_cache_algo *algo;
    struct bfdev_cache_head *head;
    unsigned long count;

    size = bfdev_pow2_roundup(size);
    if (bfdev_unlikely(size < 2))
        return NULL;

    algo = algorithm_find(name);
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
    const struct bfdev_alloc *alloc;

    algo = head->algo;
    alloc = head->alloc;

    bfdev_free(alloc, head->taghash);
    algo->destroy(head);
}

export int
bfdev_cache_register(struct bfdev_cache_algo *algo)
{
    if (!algo->name)
        return -BFDEV_EINVAL;

    if (algorithm_find(algo->name))
        return -BFDEV_EALREADY;

    bfdev_list_add(&algorithms, &algo->list);
    return -BFDEV_ENOERR;
}

export void
bfdev_cache_unregister(struct bfdev_cache_algo *algo)
{
    if (algorithm_find(algo->name))
        return;

    bfdev_list_del(&algo->list);
}
