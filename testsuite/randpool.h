/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _TESTSUITE_RANDPOOL_H_
#define _TESTSUITE_RANDPOOL_H_

#include <stdlib.h>
#include <bfdev/errno.h>
#include <bfdev/list.h>
#include <bfdev/bug.h>

typedef struct randpool_head randpool_head_t;
typedef struct randpool_node randpool_node_t;

struct randpool_head {
    bfdev_list_head_t nodes;
    unsigned int count;
};

struct randpool_node {
    bfdev_list_head_t list;
    void *data;
};

#define DEFINE_RANDPOOL(NAME)                           \
    randpool_head_t NAME = {                            \
        .nodes = BFDEV_LIST_HEAD_INIT(&(NAME).nodes),   \
        .count = 0,                                     \
    }

static inline int
randpool_put(randpool_head_t *pool, void *data)
{
    randpool_node_t *node;

    node = malloc(sizeof(*node));
    if (bfdev_unlikely(!node))
        return -BFDEV_ENOMEM;

    node->data = data;
    bfdev_list_add(&pool->nodes, &node->list);
    pool->count++;

    return -BFDEV_ENOERR;
}

static inline void *
randpool_get(randpool_head_t *pool)
{
    randpool_node_t *node;
    unsigned int index;
    void *data;

    if (!pool->count)
        return NULL;

    index = (unsigned int)rand() % pool->count;
    bfdev_list_for_each_entry(node, &pool->nodes, list) {
        if (index--)
            continue;

        bfdev_list_del(&node->list);
        pool->count--;

        data = node->data;
        free(node);

        return data;
    }

    BFDEV_BUG();
}

static inline void
randpool_release(randpool_head_t *pool, bfdev_release_t release, void *pdata)
{
    randpool_node_t *node, *tmp;

    bfdev_list_for_each_entry_safe(node, tmp, &pool->nodes, list) {
        if (release)
            release(node->data, pdata);
        free(node);
    }

    bfdev_list_head_init(&pool->nodes);
    pool->count = 0;
}

#endif /* _TESTSUITE_RANDPOOL_H_ */
