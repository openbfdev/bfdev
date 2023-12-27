/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/cache.h>
#include <bfdev/heap.h>

struct lfu_head {
    struct bfdev_cache_head cache;
    struct bfdev_heap_root lfu;
};

struct lfu_node {
    struct bfdev_cache_node cache;
    struct bfdev_heap_node node;
    unsigned long count;
};

#define cache_to_lfu_head(ptr) \
    bfdev_container_of(ptr, struct lfu_head, cache)

#define cache_to_lfu_node(ptr) \
    bfdev_container_of(ptr, struct lfu_node, cache)

#define heap_to_lfu_node(ptr) \
    bfdev_heap_entry(ptr, struct lfu_node, node)

static long
lfu_compare(const struct bfdev_heap_node *node1,
            const struct bfdev_heap_node *node2, void *pdata)
{
    struct lfu_node *lfu1, *lfu2;

    lfu1 = heap_to_lfu_node(node1);
    lfu2 = heap_to_lfu_node(node2);

    if (lfu1->count == lfu2->count)
        return 0;

    return lfu1->count < lfu2->count ? -1 : 1;
}

static bool
lfu_starving(struct bfdev_cache_head *head)
{
    struct lfu_head *lfu_head;
    lfu_head = cache_to_lfu_head(head);
    return BFDEV_HEAP_EMPTY_ROOT(&lfu_head->lfu);
}

static struct bfdev_cache_node *
lfu_obtain(struct bfdev_cache_head *head)
{
    struct lfu_head *lfu_head;
    struct lfu_node *lfu_node;

    lfu_head = cache_to_lfu_head(head);
    lfu_node = bfdev_heap_entry(BFDEV_HEAP_ROOT_NODE(&lfu_head->lfu), struct lfu_node, node);
    bfdev_heap_delete(&lfu_head->lfu, &lfu_node->node, lfu_compare, NULL);

    return &lfu_node->cache;
}

static void
lfu_get(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_head *lfu_head;
    struct lfu_node *lfu_node;

    lfu_head = cache_to_lfu_head(head);
    lfu_node = cache_to_lfu_node(node);

    bfdev_heap_delete(&lfu_head->lfu, &lfu_node->node, lfu_compare, NULL);
}

static void
lfu_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_head *lfu_head;
    struct lfu_node *lfu_node;

    lfu_head = cache_to_lfu_head(head);
    lfu_node = cache_to_lfu_node(node);

    bfdev_heap_insert(&lfu_head->lfu, &lfu_node->node, lfu_compare, NULL);
}

static void
lfu_update(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_node *lfu_node;
    lfu_node = cache_to_lfu_node(node);
    lfu_node->count++;
}

static void
lfu_clear(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_node *lfu_node;
    lfu_node = cache_to_lfu_node(node);
    lfu_node->count = 0;
}

static void
lfu_reset(struct bfdev_cache_head *head)
{
    struct lfu_head *lfu_head;
    unsigned long count;

    lfu_head = cache_to_lfu_head(head);

    bfdev_heap_init(&lfu_head->lfu);
    for (count = 0; count < head->size; ++count) {
        struct lfu_node *lfu_node;

        lfu_node = cache_to_lfu_node(head->nodes[count]);
        lfu_node->count = 0;
    }
}

static struct bfdev_cache_head *
lfu_create(const struct bfdev_alloc *alloc, unsigned long size)
{
    struct bfdev_cache_head *head;
    struct lfu_head *lfu_head;
    struct lfu_node *lfu_node;
    unsigned long count;

    lfu_head = bfdev_zalloc(alloc, sizeof(*lfu_head));
    if (bfdev_unlikely(!lfu_head))
        return NULL;

    head = &lfu_head->cache;
    bfdev_heap_init(&lfu_head->lfu);

    head->nodes = bfdev_zalloc_array(alloc, size, sizeof(*head->nodes));
    if (bfdev_unlikely(!head->nodes))
        goto free_head;

    for (count = 0; count < size; ++count) {
        lfu_node = bfdev_zalloc(alloc, sizeof(*lfu_node));
        if (bfdev_unlikely(!lfu_node))
            goto free_element;

        head->nodes[count] = &lfu_node->cache;
    }

    return head;

free_element:
    while (count--) {
        lfu_node = cache_to_lfu_node(head->nodes[count]);
        bfdev_free(alloc, lfu_node);
    }
    bfdev_free(alloc, head->nodes);

free_head:
    bfdev_free(alloc, head);
    return NULL;
}

static void
lfu_destroy(struct bfdev_cache_head *head)
{
    const struct bfdev_alloc *alloc;
    struct bfdev_cache_node *node;
    unsigned long count;

    alloc = head->alloc;

    for (count = 0; count < head->size; ++count) {
        node = head->nodes[count];
        bfdev_free(alloc, node);
    }

    bfdev_free(alloc, head->nodes);
    bfdev_free(alloc, head);
}

static struct bfdev_cache_algo
lfu_algorithm = {
    .name = "lfu",
    .starving = lfu_starving,
    .obtain = lfu_obtain,
    .get = lfu_get,
    .put = lfu_put,
    .update = lfu_update,
    .clear = lfu_clear,
    .reset = lfu_reset,
    .create = lfu_create,
    .destroy = lfu_destroy,
};

static __bfdev_ctor int
lfu_init(void)
{
    return bfdev_cache_register(&lfu_algorithm);
}
