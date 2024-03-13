/* SPDX-License-Identifier: GPL-2.0-or-later */
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
    const char *name;
    bfdev_list_head_t node;
};

struct bfdev_respool_node {
    const char *name;
    bfdev_list_head_t list;
    bfdev_respool_release_t release;
};

#define BFDEV_RESPOOL_STATIC(HEAD, NAME) { \
    .node = BFDEV_LIST_HEAD_STATIC(&(HEAD)->node), .name = (NAME), \
}

#define BFDEV_RESPOOL_INIT(head, name) \
    (bfdev_respool_t) BFDEV_RESPOOL_STATIC(head, name)

#define BFDEV_DEFINE_RESPOOL(head, name) \
    bfdev_respool_t head = BFDEV_RESPOOL_INIT(head, name)

static inline void
bfdev_respool_init(bfdev_respool_t *pool, const char *name)
{
    *pool = BFDEV_RESPOOL_INIT(pool, name);
}

static inline bool
bfdev_respool_check_empty(bfdev_respool_t *pool)
{
    return bfdev_list_check_empty(&pool->node);
}

extern bfdev_respool_node_t *
bfdev_respool_find(bfdev_respool_t *pool,
                   bfdev_respool_find_t find, void *pdata);

extern void
bfdev_respool_insert(bfdev_respool_t *pool,
                     bfdev_respool_node_t *node);

extern void
bfdev_respool_remove(bfdev_respool_t *pool,
                     bfdev_respool_node_t *node);

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
