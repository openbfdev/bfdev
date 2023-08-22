/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARRAY_H_
#define _BFDEV_ARRAY_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_ARRAY_MSIZE
# define BFDEV_ARRAY_MSIZE 32
#endif

struct bfdev_array {
    const struct bfdev_alloc *alloc;
    unsigned int capacity;
    unsigned int index;
    size_t cells;
    void *data;
};

#define BFDEV_ARRAY_STATIC(ALLOC, CELLS) \
    {.alloc = (ALLOC), .cells = (CELLS)}

#define BFDEV_ARRAY_INIT(alloc, cells) \
    (struct bfdev_array) BFDEV_ARRAY_STATIC(alloc, cells)

#define BFDEV_DEFINE_ARRAY(name, alloc, cells) \
    struct bfdev_array name = BFDEV_ARRAY_INIT(alloc, cells)

static inline void
bfdev_array_init(struct bfdev_array *array, const struct bfdev_alloc *alloc,
                 size_t cells)
{
    *array = BFDEV_ARRAY_INIT(alloc, cells);
}

static inline void
bfdev_array_reset(struct bfdev_array *array)
{
    array->index = 0;
}

static inline size_t
bfdev_array_size(struct bfdev_array *array)
{
    return array->cells * array->index;
}

extern void *
bfdev_array_push(struct bfdev_array *array, unsigned int num);

extern void *
bfdev_array_pop(struct bfdev_array *array, unsigned int num);

extern void
bfdev_array_release(struct bfdev_array *array);

BFDEV_END_DECLS

#endif /*_BFDEV_ARRAY_H_*/
