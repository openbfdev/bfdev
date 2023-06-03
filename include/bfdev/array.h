/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARRAY_H_
#define _BFDEV_ARRAY_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

struct bfdev_array {
    const struct bfdev_alloc *alloc;
    unsigned int capacity;
    unsigned int nelts;
    size_t cells;
    void *elts;
};

extern void *bfdev_array_push(struct bfdev_array *array);
extern void *bfdev_array_push_num(struct bfdev_array *array, unsigned num);

extern int bfdev_array_init(struct bfdev_array *array, const struct bfdev_alloc *alloc, unsigned num, size_t size);
extern void bfdev_array_release(struct bfdev_array *array);

extern struct bfdev_array *bfdev_array_create(const struct bfdev_alloc *alloc, unsigned int num, size_t size);
extern void bfdev_array_destroy(struct bfdev_array *array);

static inline void
bfdev_array_reset(struct bfdev_array *array)
{
    array->nelts = 0;
}

BFDEV_END_DECLS

#endif /*_BFDEV_ARRAY_H_*/
