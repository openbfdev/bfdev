/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BLOOM_H_
#define _BFDEV_BLOOM_H_

#include <bfdev/config.h>
#include <bfdev/stdbool.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

typedef unsigned int (*bfdev_bloom_hash_t)(const void *key, void *pdata);

struct bfdev_bloom {
    const struct bfdev_alloc *alloc;
    bfdev_bloom_hash_t hash;
    void *pdata;

    unsigned int capacity;
    unsigned long bitmap[0];
};

extern bool bfdev_bloom_push(struct bfdev_bloom *bloom, void *key);
extern bool bfdev_bloom_peek(struct bfdev_bloom *bloom, void *key);

extern void bfdev_bloom_clear(struct bfdev_bloom *bloom);
extern struct bfdev_bloom *bfdev_bloom_create(const struct bfdev_alloc *alloc, unsigned int capacity, bfdev_bloom_hash_t hash, void *pdata);
extern void bfdev_bloom_destory(struct bfdev_bloom *bloom);

BFDEV_END_DECLS

#endif /* _BFDEV_BLOOM_H_ */
