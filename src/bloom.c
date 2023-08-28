/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/bloom.h>
#include <bfdev/hashtbl.h>
#include <bfdev/bitops.h>
#include <export.h>

static unsigned int
bloom_index(struct bfdev_bloom *bloom, void *key)
{
    unsigned int value, index;

    value = bloom->hash(key, bloom->pdata);
    index = bfdev_hashtbl_index(bloom->capacity, value);

    return index;
}

export bool
bfdev_bloom_peek(struct bfdev_bloom *bloom, void *key)
{
    unsigned int index;

    index = bloom_index(bloom, key);
    if (bfdev_bit_test(bloom->bitmap, index))
        return true;

    return false;
}

export bool
bfdev_bloom_push(struct bfdev_bloom *bloom, void *key)
{
    unsigned int index;

    index = bloom_index(bloom, key);
    if (bfdev_bit_test_set(bloom->bitmap, index))
        return true;

    return false;
}

export bool
bfdev_bloom_clear(struct bfdev_bloom *bloom, void *key)
{
    unsigned int index;

    index = bloom_index(bloom, key);
    if (bfdev_bit_test_clr(bloom->bitmap, index))
        return true;

    return false;
}

export void
bfdev_bloom_flush(struct bfdev_bloom *bloom)
{
    size_t size = BFDEV_BITS_WORD(bloom->capacity);
    memset(bloom->bitmap, 0, size);
}

export struct bfdev_bloom *
bfdev_bloom_create(const struct bfdev_alloc *alloc, unsigned int capacity,
                   bfdev_bloom_hash_t hash, void *pdata)
{
    struct bfdev_bloom *bloom;
    size_t size;

    bfdev_align_high_adj(capacity, BFDEV_BITS_PER_LONG);
    size = BFDEV_BITS_DIV_LONG(capacity);

    bloom = bfdev_zalloc(alloc, sizeof(*bloom) + sizeof(*bloom->bitmap) * size);
    if (bfdev_unlikely(!bloom))
        return NULL;

    bloom->capacity = capacity;
    bloom->alloc = alloc;
    bloom->hash = hash;
    bloom->pdata = pdata;

    return bloom;
}

export void
bfdev_bloom_destory(struct bfdev_bloom *bloom)
{
    const struct bfdev_alloc *alloc = bloom->alloc;
    bfdev_free(alloc, bloom);
}
