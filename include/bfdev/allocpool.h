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

struct bfdev_allocpool {
    void *block;
    size_t size;
    uintptr_t last;
    unsigned long count;
};

#define BFDEV_ALLOCPOOL_STATIC(BLOCK, SIZE) \
    {.block = (BLOCK), .size = (SIZE)}

#define BFDEV_ALLOCPOOL_INIT(block, size) \
    (struct bfdev_allocpool) BFDEV_ALLOCPOOL_STATIC(block, size)

#define BFDEV_DEFINE_ALLOCPOOL(name, block, size) \
    struct bfdev_allocpool name = BFDEV_ALLOCPOOL_INIT(block, size)

/**
 * bfdev_allocpool_init() - Allocation mempool initialize.
 * @pool: minimum mempool to initialize.
 * @array: mempool array address.
 * @size: mempool array size.
 */
static inline void
bfdev_allocpool_init(struct bfdev_allocpool *pool, void *block, size_t size)
{
    *pool = BFDEV_ALLOCPOOL_INIT(block, size);
}

/**
 * bfdev_allocpool_alloc() - Allocation mempool allocation.
 * @pool: minimum mempool to alloc.
 * @size: size to allocation.
 * @align: align to allocation.
 */
extern void *
bfdev_allocpool_alloc(struct bfdev_allocpool *pool, size_t size, size_t align);

/**
 * bfdev_allocpool_free() - Allocation mempool free.
 * @pool: minimum mempool to free.
 * @block: memory block to free.
 */
extern void
bfdev_allocpool_free(struct bfdev_allocpool *pool, const char *block);

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCPOOL_H_ */
