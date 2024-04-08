/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/bloom.h>
#include <bfdev/hashtbl.h>
#include <bfdev/bitops.h>
#include <export.h>

static unsigned int
bloom_index(bfdev_bloom_t *bloom, unsigned int func, void *key)
{
    unsigned int value, index;

    value = bloom->hash(func, key, bloom->pdata);
    index = bfdev_hashtbl_index(bloom->capacity, value);

    return index;
}

export bool
bfdev_bloom_peek(bfdev_bloom_t *bloom, void *key)
{
    unsigned int index, func;
    bool retval;

    retval = true;
    for (func = 0; func < bloom->funcs; ++func) {
        index = bloom_index(bloom, func, key);
        if (!bfdev_bit_test(bloom->bitmap, index))
            retval = false;
    }

    return retval;
}

export bool
bfdev_bloom_push(bfdev_bloom_t *bloom, void *key)
{
    unsigned int index, func;
    bool retval;

    retval = true;
    for (func = 0; func < bloom->funcs; ++func) {
        index = bloom_index(bloom, func, key);
        if (!bfdev_bit_test_set(bloom->bitmap, index))
            retval = false;
    }

    return retval;
}

export void
bfdev_bloom_flush(bfdev_bloom_t *bloom)
{
    size_t size;

    size = BFDEV_BITS_WORD(bloom->capacity);
    bfport_memset(bloom->bitmap, 0, size);
}

export bfdev_bloom_t *
bfdev_bloom_create(const bfdev_alloc_t *alloc, unsigned int capacity,
                   bfdev_bloom_hash_t hash, unsigned int funcs, void *pdata)
{
    bfdev_bloom_t *bloom;
    size_t size;

    bfdev_align_high_adj(capacity, BFDEV_BITS_PER_LONG);
    size = BFDEV_BITS_DIV_LONG(capacity);

    bloom = bfdev_zalloc(alloc, sizeof(*bloom) + sizeof(*bloom->bitmap) * size);
    if (bfdev_unlikely(!bloom))
        return NULL;

    bloom->capacity = capacity;
    bloom->alloc = alloc;
    bloom->hash = hash;
    bloom->funcs = funcs;
    bloom->pdata = pdata;

    return bloom;
}

export void
bfdev_bloom_destory(bfdev_bloom_t *bloom)
{
    const bfdev_alloc_t *alloc;

    alloc = bloom->alloc;
    bfdev_free(alloc, bloom);
}
