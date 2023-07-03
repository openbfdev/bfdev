/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MINPOOL_H_
#define _BFDEV_MINPOOL_H_

#include <bfdev/config.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_MINPOOL_ALIGN
# define BFDEV_MINPOOL_ALIGN 16
#endif

struct bfdev_minpool_head {
    struct bfdev_list_head block_list;
    struct bfdev_list_head free_list;
    size_t avail;
};

/**
 * bfdev_minpool_alloc - Minimum mempool allocation.
 * @head: Minimum mempool to operate.
 * @size: Size to allocation.
 */
extern void __bfdev_malloc *
bfdev_minpool_alloc(struct bfdev_minpool_head *head, size_t size);

/**
 * bfdev_minpool_alloc - Minimum mempool reallocation.
 * @head: Minimum mempool to operate.
 * @block: Block address to realloc.
 * @size: Size to realloc.
 */
extern void __bfdev_malloc *
bfdev_minpool_realloc(struct bfdev_minpool_head *head, void *block, size_t resize);

/**
 * bfdev_minpool_free - Minimum mempool release.
 * @head: Minimum mempool to operate.
 * @block: Block address to free.
 */
extern void
bfdev_minpool_free(struct bfdev_minpool_head *head, void *block);

/**
 * minpool_setup - Minimum mempool setup.
 * @head: Minimum mempool to operate.
 * @array: Mempool array address.
 * @size: Mempool array size.
 */
extern void
bfdev_minpool_setup(struct bfdev_minpool_head *head, void *array, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_MINPOOL_H_ */
