/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_GALLOCATOR_H_
#define _BFDEV_GALLOCATOR_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void *bfdev_galloc_malloc(size_t size, void *pdata);
extern void *bfdev_galloc_realloc(const void *block, size_t resize, void *pdata);
extern void bfdev_galloc_free(const void *block, void *pdata);
extern const struct bfdev_alloc bfdev_galloc;

static inline const struct bfdev_alloc *bfdev_galloc_create(void)
{
    return &bfdev_galloc;
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_GALLOCATOR_H_ */
