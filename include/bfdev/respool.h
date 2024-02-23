/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RESPOOL_H_
#define _BFDEV_RESPOOL_H_

#include <bfdev/config.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_resnode bfdev_resnode_t;
typedef struct bfdev_respool bfdev_respool_t;

typedef void (*bfdev_respool_release_t)
(bfdev_respool_t *pool, bfdev_resnode_t *res);

typedef long (*bfdev_respool_find_t)
(bfdev_respool_t *pool, bfdev_resnode_t *res, const void *data);

struct bfdev_resnode {
    const char *name;
    bfdev_list_head_t list;
    bfdev_respool_release_t release;
};

struct bfdev_respool {
    const char *name;
    bfdev_list_head_t node;
};

#define BFDEV_RESPOOL_STATIC(name) {                \
    .name = __bfdev_stringify(name),                \
    .node = BFDEV_LIST_HEAD_STATIC((name).list),    \
}

#define BFDEV_RESPOOL_INIT(name) \
    (bfdev_respool_t) BFDEV_RESPOOL_STATIC(name)

#define BFDEV_DEFINE_RESPOOL(name) \
    bfdev_respool_t name = BFDEV_RESPOOL_INIT(name)

static inline void
bfdev_respool_init(bfdev_respool_t *pool, const char *name)
{
    pool->name = name;
    bfdev_list_head_init(&pool->node);
}

static inline bool
bfdev_respool_check_empty(bfdev_respool_t *pool)
{
    return bfdev_list_check_empty(&pool->node);
}

extern bfdev_resnode_t *
bfdev_respool_find(bfdev_respool_t *pool, bfdev_respool_find_t find,
                   const void *data);

extern void
bfdev_respool_insert(bfdev_respool_t *pool, bfdev_resnode_t *res);

extern void
bfdev_respool_remove(bfdev_respool_t *pool, bfdev_resnode_t *res);

extern void
bfdev_respool_release(bfdev_respool_t *pool, bfdev_resnode_t *res);

extern bfdev_resnode_t *
bfdev_respool_find_remove(bfdev_respool_t *pool, bfdev_respool_find_t find,
                          const void *data);

extern bfdev_resnode_t *
bfdev_respool_find_release(bfdev_respool_t *pool, bfdev_respool_find_t find,
                           const void *data);

extern void
bfdev_respool_release_all(bfdev_respool_t *pool);

BFDEV_END_DECLS

#endif /* _BFDEV_RESPOOL_H_ */
