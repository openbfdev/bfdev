/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RESPOOL_H_
#define _BFDEV_RESPOOL_H_

#include <bfdev/config.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_respool bfdev_respool_t;
typedef struct bfdev_respool_node bfdev_respool_node_t;

BFDEV_CALLBACK_FIND(
    bfdev_respool_find_t,
    bfdev_respool_node_t *
);

BFDEV_CALLBACK_RELEASE(
    bfdev_respool_release_t,
    bfdev_respool_node_t *
);

struct bfdev_respool {
    bfdev_list_head_t nodes;
};

struct bfdev_respool_node {
    bfdev_list_head_t list;
    bfdev_respool_release_t release;
};

#define BFDEV_RESPOOL_STATIC(HEAD) { \
    .nodes = BFDEV_LIST_HEAD_STATIC(&(HEAD)->nodes), \
}

#define BFDEV_RESPOOL_INIT(head) \
    (bfdev_respool_t) BFDEV_RESPOOL_STATIC(head)

#define BFDEV_DEFINE_RESPOOL(name) \
    bfdev_respool_t name = BFDEV_RESPOOL_INIT(&name)

static inline void
bfdev_respool_init(bfdev_respool_t *pool)
{
    *pool = BFDEV_RESPOOL_INIT(pool);
}

static inline bool
bfdev_respool_check_empty(bfdev_respool_t *pool)
{
    return bfdev_list_check_empty(&pool->nodes);
}

static inline void
bfdev_respool_insert(bfdev_respool_t *pool,
                     bfdev_respool_node_t *node)
{
    bfdev_list_add(&pool->nodes, &node->list);
}

static inline void
bfdev_respool_remove(bfdev_respool_t *pool,
                     bfdev_respool_node_t *node)
{
    bfdev_list_del(&node->list);
}

extern bfdev_respool_node_t *
bfdev_respool_find(bfdev_respool_t *pool,
                   bfdev_respool_find_t find, void *pdata);

extern void
bfdev_respool_release(bfdev_respool_t *pool,
                      bfdev_respool_node_t *node, void *pdata);

extern bfdev_respool_node_t *
bfdev_respool_find_remove(bfdev_respool_t *pool,
                          bfdev_respool_find_t find, void *pdata);

extern bfdev_respool_node_t *
bfdev_respool_find_release(bfdev_respool_t *pool,
                           bfdev_respool_find_t find, void *pdata);

extern void
bfdev_respool_release_all(bfdev_respool_t *pool, void *pdata);

BFDEV_END_DECLS

#endif /* _BFDEV_RESPOOL_H_ */
