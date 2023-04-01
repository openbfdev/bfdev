/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
* Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
*/

#ifndef _BFDEV_ARRAY_H_
#define _BFDEV_ARRAY_H_

#include <bfdev/stddef.h>
#include <bfdev/errno.h>
#include <bfdev/string.h>
#include <bfdev/allocator.h>
#include <bfdev/galloctor.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bfdev_array_s bfdev_array_t;

struct bfdev_array_s{
    void *elts;
    unsigned nelts;
    size_t size;
    unsigned nalloc;
    allocator_t *allocator;
};

bfdev_array_t *bfdev_array_create(allocator_t *allocator, unsigned n, size_t size);
void bfdev_array_destroy(bfdev_array_t *a);
void *bfdev_array_push(bfdev_array_t *a);
void *bfdev_array_push_n(bfdev_array_t *a, unsigned n);

static inline void
bfdev_array_reset(bfdev_array_t *a)
{
    a->nelts = 0;
}

#ifdef __cplusplus
}
#endif

#endif /*_BFDEV_ARRAY_H_*/
