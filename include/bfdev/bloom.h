/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BLOOM_H_
#define _BFDEV_BLOOM_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_bloom bfdev_bloom_t;

typedef unsigned int (*bfdev_bloom_hash_t)
(unsigned int func, const void *key, void *pdata);

struct bfdev_bloom {
    const struct bfdev_alloc *alloc;
    bfdev_bloom_hash_t hash;
    unsigned int funcs;
    void *pdata;

    unsigned int capacity;
    unsigned long bitmap[];
};

/**
 * bfdev_bloom_peek() - peek an object from a bloom filter.
 * @bloom: bloom filter pointer.
 * @key: object pointer.
 *
 * @return: object value.
 */
extern bool
bfdev_bloom_peek(bfdev_bloom_t *bloom, void *key);

/**
 * bfdev_bloom_push() - push an object from a bloom filter.
 * @bloom: bloom filter pointer.
 * @key: object pointer to push.
 *
 * @return: object value before push.
 */
extern bool
bfdev_bloom_push(bfdev_bloom_t *bloom, void *key);

/**
 * bfdev_bloom_flush() - flush the entire bloom filter.
 * @bloom: bloom filter pointer.
 */
extern void
bfdev_bloom_flush(bfdev_bloom_t *bloom);

/**
 * bfdev_bloom_create() - creat a bloom filter.
 * @capacity: capacity size of bloom filter.
 * @hash: object hash callback function.
 * @funcs: number of supported hash algorithms.
 * @pdata: private data pointer of @hash.
 */
extern bfdev_bloom_t *
bfdev_bloom_create(const struct bfdev_alloc *alloc, unsigned int capacity,
                   bfdev_bloom_hash_t hash, unsigned int funcs, void *pdata);

/**
 * bfdev_bloom_destory() - destory a bloom filter.
 * @bloom: bloom filter pointer.
 */
extern void
bfdev_bloom_destory(bfdev_bloom_t *bloom);

BFDEV_END_DECLS

#endif /* _BFDEV_BLOOM_H_ */
