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

struct bfdev_minpool_head;
struct bfdev_minpool_node;

typedef struct bfdev_minpool_node *
(*bfdev_minpool_find_t)(struct bfdev_minpool_head *head, size_t size);

struct bfdev_minpool_head {
    bfdev_list_head_t block_list;
    bfdev_list_head_t free_list;
    bfdev_minpool_find_t find;
    size_t avail;
};

struct bfdev_minpool_node {
    bfdev_list_head_t block;
    bfdev_list_head_t free;
    size_t usize;
    char data[0];
};

/**
 * minpool_first_fit - Get first qualified node in mempool.
 * @head: Minimum mempool to get node.
 * @size: Node minimum size to get.
 */
extern struct bfdev_minpool_node *
bfdev_minpool_first_fit(struct bfdev_minpool_head *head, size_t size);

/**
 * bfdev_minpool_best_fit - Get best qualified node in mempool.
 * @head: Minimum mempool to get node.
 * @size: Node minimum size to get.
 */
extern struct bfdev_minpool_node *
bfdev_minpool_best_fit(struct bfdev_minpool_head *head, size_t size);

/**
 * bfdev_minpool_worst_fit - Get worst qualified node in mempool.
 * @head: Minimum mempool to get node.
 * @size: Node minimum size to get.
 */
extern struct bfdev_minpool_node *
bfdev_minpool_worst_fit(struct bfdev_minpool_head *head, size_t size);

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
bfdev_minpool_setup(struct bfdev_minpool_head *head, bfdev_minpool_find_t find,
                    void *array, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_MINPOOL_H_ */
