/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/respool.h>
#include <export.h>

export bfdev_respool_node_t *
bfdev_respool_find(bfdev_respool_t *pool,
                   bfdev_respool_find_t find, void *data)
{
    bfdev_respool_node_t *walk;

    bfdev_list_for_each_entry(walk, &pool->nodes, list) {
        if (!find(walk, data))
            return walk;
    }

    return NULL;
}

export void
bfdev_respool_release(bfdev_respool_t *pool,
                      bfdev_respool_node_t *node, void *pdata)
{
    bfdev_list_del(&node->list);
    node->release(node, pdata);
}

export bfdev_respool_node_t *
bfdev_respool_find_remove(bfdev_respool_t *pool,
                          bfdev_respool_find_t find, void *data)
{
    bfdev_respool_node_t *match;

    match = bfdev_respool_find(pool, find, data);
    if (bfdev_likely(match))
        bfdev_list_del(&match->list);

    return match;
}

export bfdev_respool_node_t *
bfdev_respool_find_release(bfdev_respool_t *pool,
                           bfdev_respool_find_t find, void *pdata)
{
    bfdev_respool_node_t *match;

    match = bfdev_respool_find(pool, find, pdata);
    if (bfdev_likely(match)) {
        bfdev_list_del(&match->list);
        match->release(match, pdata);
    }

    return match;
}

export void
bfdev_respool_release_all(bfdev_respool_t *pool, void *pdata)
{
    bfdev_respool_node_t *node;

    bfdev_list_for_each_entry(node, &pool->nodes, list)
        node->release(node, pdata);
}
