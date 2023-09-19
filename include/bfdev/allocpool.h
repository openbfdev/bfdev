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

/**
 * bfdev_allocpool_alloc() - Allocation mempool allocation.
 * @pool: minimum mempool to operate.
 * @size: size to allocation.
 * @align: align to allocation.
 */
extern void *
bfdev_allocpool_alloc(struct bfdev_allocpool *pool, size_t size, size_t align);

/**
 * bfdev_allocpool_free() - Allocation mempool free.
 * @pool: minimum mempool to operate.
 */
extern void
bfdev_allocpool_free(struct bfdev_allocpool *pool);

/**
 * bfdev_allocpool_setup() - Allocation mempool setup.
 * @pool: minimum mempool to operate.
 * @array: mempool array address.
 * @size: mempool array size.
 */
extern void
bfdev_allocpool_setup(struct bfdev_allocpool *pool, void *block, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCPOOL_H_ */
