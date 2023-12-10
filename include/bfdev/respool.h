/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RESPOOL_H_
#define _BFDEV_RESPOOL_H_

#include <bfdev/config.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

struct bfdev_resnode;
struct bfdev_respool;

typedef void (*bfdev_respool_release_t)
(struct bfdev_respool *pool, struct bfdev_resnode *res);

typedef long (*bfdev_respool_find_t)
(struct bfdev_respool *pool, struct bfdev_resnode *res, const void *data);

struct bfdev_resnode {
    const char *name;
    struct bfdev_list_head list;
    bfdev_respool_release_t release;
};

struct bfdev_respool {
    const char *name;
    struct bfdev_list_head node;
};

#define BFDEV_RESPOOL_STATIC(name) {                \
    .name = __bfdev_stringify(name),                \
    .node = BFDEV_LIST_HEAD_STATIC((name).list),    \
}

#define BFDEV_RESPOOL_INIT(name) \
    (struct bfdev_respool) BFDEV_RESPOOL_STATIC(name)

#define BFDEV_DEFINE_RESPOOL(name) \
    struct bfdev_respool name = BFDEV_RESPOOL_INIT(name)

static inline void
bfdev_respool_init(struct bfdev_respool *pool, const char *name)
{
    pool->name = name;
    bfdev_list_head_init(&pool->node);
}

static inline bool
bfdev_respool_check_empty(struct bfdev_respool *pool)
{
    return bfdev_list_check_empty(&pool->node);
}

extern struct bfdev_resnode *
bfdev_respool_find(struct bfdev_respool *pool,
                   bfdev_respool_find_t find, const void *data);

extern void
bfdev_respool_insert(struct bfdev_respool *pool, struct bfdev_resnode *res);

extern void
bfdev_respool_remove(struct bfdev_respool *pool, struct bfdev_resnode *res);

extern void
bfdev_respool_release(struct bfdev_respool *pool, struct bfdev_resnode *res);

extern struct bfdev_resnode *
bfdev_respool_find_remove(struct bfdev_respool *pool, bfdev_respool_find_t find,
                          const void *data);

extern struct bfdev_resnode *
bfdev_respool_find_release(struct bfdev_respool *pool, bfdev_respool_find_t find,
                           const void *data);

extern void
bfdev_respool_release_all(struct bfdev_respool *pool);

BFDEV_END_DECLS

#endif /* _BFDEV_RESPOOL_H_ */
