/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARRAY_H_
#define _BFDEV_ARRAY_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_ARRAY_MSIZE
# define BFDEV_ARRAY_MSIZE 32
#endif

typedef struct bfdev_array bfdev_array_t;

struct bfdev_array {
    const struct bfdev_alloc *alloc;
    unsigned long capacity;
    unsigned long index;
    size_t cells;
    void *data;
};

#define BFDEV_ARRAY_STATIC(ALLOC, CELLS) \
    {.alloc = (ALLOC), .cells = (CELLS)}

#define BFDEV_ARRAY_INIT(alloc, cells) \
    (bfdev_array_t) BFDEV_ARRAY_STATIC(alloc, cells)

#define BFDEV_DEFINE_ARRAY(name, alloc, cells) \
    bfdev_array_t name = BFDEV_ARRAY_INIT(alloc, cells)

static inline void
bfdev_array_init(bfdev_array_t *array, const struct bfdev_alloc *alloc,
                 size_t cells)
{
    *array = BFDEV_ARRAY_INIT(alloc, cells);
}

static inline void
bfdev_array_reset(bfdev_array_t *array)
{
    array->index = 0;
}

static inline unsigned long
bfdev_array_index(bfdev_array_t *array)
{
    return array->index;
}

static inline size_t
bfdev_array_size(bfdev_array_t *array)
{
    return array->cells * array->index;
}

static inline uintptr_t
bfdev_array_offset(bfdev_array_t *array, unsigned long index)
{
    return array->cells * index;
}

static inline void *
bfdev_array_data(bfdev_array_t *array, unsigned long index)
{
    if (bfdev_unlikely(index >= array->index))
        return NULL;
    return array->data + bfdev_array_offset(array, index);
}

extern void *
bfdev_array_push(bfdev_array_t *array, unsigned long num);

extern void *
bfdev_array_pop(bfdev_array_t *array, unsigned long num);

extern int
bfdev_array_reserve(bfdev_array_t *array, unsigned long num);

extern void
bfdev_array_release(bfdev_array_t *array);

BFDEV_END_DECLS

#endif /*_BFDEV_ARRAY_H_*/
