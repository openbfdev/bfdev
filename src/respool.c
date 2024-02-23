/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-respool"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/respool.h>
#include <bfdev/log.h>
#include <export.h>

export bfdev_resnode_t *
bfdev_respool_find(bfdev_respool_t *pool, bfdev_respool_find_t find,
                   const void *data)
{
    bfdev_resnode_t *walk;

    bfdev_list_for_each_entry(walk, &pool->node, list) {
        if (!find(pool, walk, data))
            return walk;
    }

    return NULL;
}

export void
bfdev_respool_insert(bfdev_respool_t *pool, bfdev_resnode_t *node)
{
    bfdev_list_add_prev(&pool->node, &node->list);
    bfdev_log_debug("%s: insert %p '%s'\n",
                    pool->name, node, node->name);
}

export void
bfdev_respool_remove(bfdev_respool_t *pool, bfdev_resnode_t *node)
{
    bfdev_list_del(&node->list);
    bfdev_log_debug("%s: remove %p '%s'\n",
                    pool->name, node, node->name);
}

export void
bfdev_respool_release(bfdev_respool_t *pool, bfdev_resnode_t *node)
{
    bfdev_list_del(&node->list);
    node->release(pool, node);
    bfdev_log_debug("%s: release %p '%s'\n",
                    pool->name, node, node->name);
}

export bfdev_resnode_t *
bfdev_respool_find_remove(bfdev_respool_t *pool, bfdev_respool_find_t find,
                          const void *data)
{
    bfdev_resnode_t *match;

    match = bfdev_respool_find(pool, find, data);
    if (bfdev_likely(match))
        bfdev_list_del(&match->list);

    if (bfdev_likely(match)) {
        bfdev_log_debug("%s: find-remove %p '%s'\n",
                        pool->name, match, match->name);
    }

    return match;
}

export bfdev_resnode_t *
bfdev_respool_find_release(bfdev_respool_t *pool, bfdev_respool_find_t find,
                           const void *data)
{
    bfdev_resnode_t *match;

    match = bfdev_respool_find(pool, find, data);
    if (bfdev_likely(match)) {
        bfdev_list_del(&match->list);
        match->release(pool, match);
    }

    if (bfdev_likely(match)) {
        bfdev_log_debug("%s: find-release %p '%s'\n",
                        pool->name, match, match->name);
    }

    return match;
}

export void
bfdev_respool_release_all(bfdev_respool_t *pool)
{
    bfdev_resnode_t *node;

    bfdev_list_for_each_entry(node, &pool->node, list) {
        node->release(pool, node);
        bfdev_log_debug("%s: release-all %p '%s'\n",
                        pool->name, node, node->name);
    }
}
