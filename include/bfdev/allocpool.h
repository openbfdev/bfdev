/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALLOCPOOL_H_
#define _BFDEV_ALLOCPOOL_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_allocpool bfdev_allocpool_t;

struct bfdev_allocpool {
    void *block;
    size_t size;
    uintptr_t last;
    unsigned long count;
};

#define BFDEV_ALLOCPOOL_STATIC(BLOCK, SIZE) { \
    .block = (BLOCK), .size = (SIZE), \
}

#define BFDEV_ALLOCPOOL_INIT(block, size) \
    (bfdev_allocpool_t) BFDEV_ALLOCPOOL_STATIC(block, size)

#define BFDEV_DEFINE_ALLOCPOOL(name, block, size) \
    bfdev_allocpool_t name = BFDEV_ALLOCPOOL_INIT(block, size)

/**
 * bfdev_allocpool_init() - Allocation mempool initialize.
 * @pool: the allocpool to initialize.
 * @array: mempool array address.
 * @size: mempool array size.
 */
static inline void
bfdev_allocpool_init(bfdev_allocpool_t *pool, void *block, size_t size)
{
    *pool = BFDEV_ALLOCPOOL_INIT(block, size);
}

/**
 * bfdev_allocpool_reset() - Allocation mempool reset.
 * @pool: the allocpool to reset.
 */
static inline void
bfdev_allocpool_reset(bfdev_allocpool_t *pool)
{
    pool->last = 0;
    pool->count = 0;
}

/**
 * bfdev_allocpool_alloc() - Allocation mempool allocation.
 * @pool: the allocpool to alloc.
 * @size: size to allocation.
 * @align: align to allocation.
 */
extern __bfdev_malloc void *
bfdev_allocpool_alloc(bfdev_allocpool_t *pool, size_t size, size_t align);

/**
 * bfdev_allocpool_free() - Allocation mempool free.
 * @pool: the allocpool to free.
 * @block: memory block to free.
 */
extern void
bfdev_allocpool_free(bfdev_allocpool_t *pool, const void *block);

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCPOOL_H_ */
