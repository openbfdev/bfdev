/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MEMALLOC_H_
#define _BFDEV_MEMALLOC_H_

#include <bfdev/config.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_MEMALLOC_ALIGN
# define BFDEV_MEMALLOC_ALIGN 32
#endif

typedef struct bfdev_memalloc_head bfdev_memalloc_head_t;
typedef struct bfdev_memalloc_chunk bfdev_memalloc_chunk_t;

typedef bfdev_memalloc_chunk_t *
(*bfdev_memalloc_find_t)(bfdev_memalloc_head_t *head, size_t size);

struct bfdev_memalloc_head {
    bfdev_list_head_t block_list;
    bfdev_list_head_t free_list;
    bfdev_memalloc_find_t find;
    size_t avail;
};

struct bfdev_memalloc_chunk {
    bfdev_list_head_t block;
    bfdev_list_head_t free;
    size_t usize;
    char data[0];
};

/**
 * bfdev_memalloc_first_fit() - first qualified node.
 * @head: memalloc to get node.
 * @size: size to get.
 */
extern bfdev_memalloc_chunk_t *
bfdev_memalloc_first_fit(bfdev_memalloc_head_t *head, size_t size);

/**
 * bfdev_memalloc_best_fit() - best qualified node.
 * @head: memalloc to get node.
 * @size: size to get.
 */
extern bfdev_memalloc_chunk_t *
bfdev_memalloc_best_fit(bfdev_memalloc_head_t *head, size_t size);

/**
 * bfdev_memalloc_worst_fit() - worst qualified node.
 * @head: memalloc to get node.
 * @size: size to get.
 */
extern bfdev_memalloc_chunk_t *
bfdev_memalloc_worst_fit(bfdev_memalloc_head_t *head, size_t size);

/**
 * bfdev_memalloc_alloc() - memory allocator allocation.
 * @head: memalloc to operate.
 * @size: size to allocation.
 */
extern void __bfdev_malloc *
bfdev_memalloc_alloc(bfdev_memalloc_head_t *head, size_t size);

/**
 * bfdev_memalloc_free() - memory allocator release.
 * @head: memalloc to operate.
 * @block: memory to free.
 */
extern void
bfdev_memalloc_free(bfdev_memalloc_head_t *head, void *block);

/**
 * bfdev_memalloc_realloc() - memory allocator reallocation.
 * @head: memalloc to operate.
 * @block: memory to realloc.
 * @resize: size to realloc.
 */
extern void __bfdev_malloc *
bfdev_memalloc_realloc(bfdev_memalloc_head_t *head, void *block, size_t resize);

/**
 * bfdev_memalloc_init() - memory allocator setup.
 * @head: memalloc to operate.
 * @find: memalloc allocator algorithm.
 * @memory: memalloc memory address.
 * @size: memalloc memory size.
 */
extern void
bfdev_memalloc_init(bfdev_memalloc_head_t *head, bfdev_memalloc_find_t find,
                    void *memory, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_MEMALLOC_H_ */
