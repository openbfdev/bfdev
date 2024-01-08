/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/cache.h>

struct lru_head {
    struct bfdev_cache_head cache;
    bfdev_list_head_t lru;
};

struct lru_node {
    struct bfdev_cache_node cache;
    bfdev_list_head_t node;
};

#define cache_to_lru_head(ptr) \
    bfdev_container_of(ptr, struct lru_head, cache)

#define cache_to_lru_node(ptr) \
    bfdev_container_of(ptr, struct lru_node, cache)

static bool
lru_starving(struct bfdev_cache_head *head)
{
    struct lru_head *lru_head;
    lru_head = cache_to_lru_head(head);
    return bfdev_list_check_empty(&lru_head->lru);
}

static struct bfdev_cache_node *
lru_obtain(struct bfdev_cache_head *head)
{
    struct lru_head *lru_head;
    struct lru_node *lru_node;

    lru_head = cache_to_lru_head(head);
    lru_node = bfdev_list_last_entry(&lru_head->lru, struct lru_node, node);
    bfdev_list_del(&lru_node->node);

    return &lru_node->cache;
}

static void
lru_get(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lru_node *lru_node;
    lru_node = cache_to_lru_node(node);
    bfdev_list_del(&lru_node->node);
}

static void
lru_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lru_head *lru_head;
    struct lru_node *lru_node;

    lru_head = cache_to_lru_head(head);
    lru_node = cache_to_lru_node(node);

    bfdev_list_add(&lru_head->lru, &lru_node->node);
}

static void
lru_reset(struct bfdev_cache_head *head)
{
    struct lru_head *lru_head;
    lru_head = cache_to_lru_head(head);
    bfdev_list_head_init(&lru_head->lru);
}

static struct bfdev_cache_head *
lru_create(const bfdev_alloc_t *alloc, unsigned long size)
{
    struct bfdev_cache_head *head;
    struct lru_head *lru_head;
    struct lru_node *lru_node;
    unsigned long count;

    lru_head = bfdev_zalloc(alloc, sizeof(*lru_head));
    if (bfdev_unlikely(!lru_head))
        return NULL;

    head = &lru_head->cache;
    bfdev_list_head_init(&lru_head->lru);

    head->nodes = bfdev_zalloc_array(alloc, size, sizeof(*head->nodes));
    if (bfdev_unlikely(!head->nodes))
        goto free_head;

    for (count = 0; count < size; ++count) {
        lru_node = bfdev_zalloc(alloc, sizeof(*lru_node));
        if (bfdev_unlikely(!lru_node))
            goto free_element;

        head->nodes[count] = &lru_node->cache;
    }

    return head;

free_element:
    while (count--) {
        lru_node = cache_to_lru_node(head->nodes[count]);
        bfdev_free(alloc, lru_node);
    }
    bfdev_free(alloc, head->nodes);

free_head:
    bfdev_free(alloc, head);
    return NULL;
}

static void
lru_destroy(struct bfdev_cache_head *head)
{
    struct lru_head *lru_head = cache_to_lru_head(head);
    const bfdev_alloc_t *alloc;
    struct bfdev_cache_node *node;
    unsigned long count;

    alloc = head->alloc;
    for (count = 0; count < head->size; ++count) {
        node = head->nodes[count];
        bfdev_free(alloc, node);
    }

    bfdev_free(alloc, head->nodes);
    bfdev_free(alloc, lru_head);
}

static struct bfdev_cache_algo
lru_algorithm = {
    .name = "lru",
    .starving = lru_starving,
    .obtain = lru_obtain,
    .get = lru_get,
    .put = lru_put,
    .reset = lru_reset,
    .create = lru_create,
    .destroy = lru_destroy,
};

static __bfdev_ctor int
lru_init(void)
{
    return bfdev_cache_register(&lru_algorithm);
}
