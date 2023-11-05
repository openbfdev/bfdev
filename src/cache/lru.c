/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/cache.h>

struct lru_head {
    struct bfdev_cache_head cache;
    struct bfdev_list_head lru;
};

#define head_to_cache(ptr) \
    bfdev_container_of(ptr, struct lru_head, cache)

static bool
lru_starving(struct bfdev_cache_head *head)
{
    struct lru_head *lhead = head_to_cache(head);
    return bfdev_list_check_empty(&lhead->lru);
}

static struct bfdev_cache_node *
lru_obtain(struct bfdev_cache_head *head)
{
    struct lru_head *lhead = head_to_cache(head);
    return bfdev_list_last_entry(&lhead->lru, struct bfdev_cache_node, list);
}

static void
lru_put(struct bfdev_cache_head *head, struct bfdev_cache_node *node)
{
    struct lru_head *lhead = head_to_cache(head);
    bfdev_list_move(&lhead->lru, &node->list);
}

static void
lru_reset(struct bfdev_cache_head *head)
{
    struct lru_head *lhead = head_to_cache(head);
    bfdev_list_head_init(&lhead->lru);
}

static struct bfdev_cache_head *
lru_create(const struct bfdev_alloc *alloc, unsigned long size)
{
    struct bfdev_cache_node *node;
    struct bfdev_cache_head *head;
    struct lru_head *lhead;
    unsigned long count;

    lhead = bfdev_zalloc(alloc, sizeof(*lhead));
    if (bfdev_unlikely(!lhead))
        return NULL;

    head = &lhead->cache;
    bfdev_list_head_init(&lhead->lru);

    head->nodes = bfdev_zalloc_array(alloc, size, sizeof(*head->nodes));
    if (bfdev_unlikely(!head->nodes))
        goto free_head;

    for (count = 0; count < size; ++count) {
        node = bfdev_zalloc(alloc, sizeof(*node));
        if (bfdev_unlikely(!node))
            goto free_element;

        head->nodes[count] = node;
    }

    return head;

free_element:
    while (count--) {
        node = head->nodes[count];
        bfdev_free(alloc, node);
    }
    bfdev_free(alloc, head->nodes);

free_head:
    bfdev_free(alloc, head);
    return NULL;
}

static void
lru_destroy(struct bfdev_cache_head *head)
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
lru_algorithm = {
    .name = "lru",
    .starving = lru_starving,
    .obtain = lru_obtain,
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
