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

#define cache_to_head(ptr) \
    bfdev_container_of(ptr, struct lfu_head, cache)

#define cache_to_node(ptr) \
    bfdev_container_of(ptr, struct lfu_node, cache)

#define heap_to_node(ptr) \
    bfdev_container_of(ptr, struct lfu_node, node)

static long
lfu_compare(const struct bfdev_heap_node *node1,
            const struct bfdev_heap_node *node2, void *pdata)
{
    struct lfu_node *lfu1, *lfu2;

    lfu1 = heap_to_node(node1);
    lfu2 = heap_to_node(node2);

    if (lfu1->count == lfu2->count)
        return 0;

    return lfu1->count < lfu2->count ? -1 : 1;
}

static bool
lfu_starving(struct bfdev_cache_head *head)
{
    struct lfu_head *lhead;
    lhead = cache_to_head(head);
    return BFDEV_HEAP_EMPTY_ROOT(&lhead->lfu);
}

static struct bfdev_cache_node *
lfu_obtain(struct bfdev_cache_head *head)
{
    struct lfu_head *lhead;
    struct lfu_node *lnode;

    lhead = cache_to_head(head);
    lnode = bfdev_heap_entry(BFDEV_HEAP_ROOT_NODE(&lhead->lfu), struct lfu_node, node);
    bfdev_heap_delete(&lhead->lfu, &lnode->node, lfu_compare, NULL);

    return &lnode->cache;
}

static void
lfu_get(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_head *lhead;
    struct lfu_node *lnode;

    lhead = cache_to_head(head);
    lnode = cache_to_node(node);

    lnode->count++;
    bfdev_heap_delete(&lhead->lfu, &lnode->node, lfu_compare, NULL);
}

static void
lfu_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_head *lhead;
    struct lfu_node *lnode;

    lhead = cache_to_head(head);
    lnode = cache_to_node(node);

    bfdev_heap_insert(&lhead->lfu, &lnode->node, lfu_compare, NULL);
}

static void
lfu_update(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_head *lhead;
    struct lfu_node *lnode;

    lhead = cache_to_head(head);
    lnode = cache_to_node(node);

    lnode->count++;
    bfdev_heap_fixup(&lhead->lfu, &lnode->node, lfu_compare, NULL);
}

static void
lfu_clear(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lfu_node *lnode;
    lnode = cache_to_node(node);
    lnode->count = 0;
}

static void
lfu_reset(struct bfdev_cache_head *head)
{
    struct lfu_head *lhead;
    unsigned long count;

    lhead = cache_to_head(head);

    bfdev_heap_init(&lhead->lfu);
    for (count = 0; count < head->size; ++count) {
        struct lfu_node *lnode;

        lnode = cache_to_node(head->nodes[count]);
        lnode->count = 0;
    }
}

static struct bfdev_cache_head *
lfu_create(const struct bfdev_alloc *alloc, unsigned long size)
{
    struct bfdev_cache_head *head;
    struct lfu_head *lhead;
    struct lfu_node *lnode;
    unsigned long count;

    lhead = bfdev_zalloc(alloc, sizeof(*lhead));
    if (bfdev_unlikely(!lhead))
        return NULL;

    head = &lhead->cache;
    bfdev_heap_init(&lhead->lfu);

    head->nodes = bfdev_zalloc_array(alloc, size, sizeof(*head->nodes));
    if (bfdev_unlikely(!head->nodes))
        goto free_head;

    for (count = 0; count < size; ++count) {
        lnode = bfdev_zalloc(alloc, sizeof(*lnode));
        if (bfdev_unlikely(!lnode))
            goto free_element;

        head->nodes[count] = &lnode->cache;
    }

    return head;

free_element:
    while (count--) {
        lnode = cache_to_node(head->nodes[count]);
        bfdev_free(alloc, lnode);
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
