/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/array.h>
#include <bfdev/overflow.h>
#include <export.h>

static inline int
array_resize(bfdev_array_t *array, unsigned long count)
{
    const bfdev_alloc_t *alloc;
    unsigned long nalloc;
    size_t size;
    void *data;

    nalloc = bfdev_max(count, BFDEV_ARRAY_MSIZE);
    size = nalloc * array->cells;

    alloc = array->alloc;
    data = bfdev_realloc(alloc, array->data, size);
    if (bfdev_unlikely(!data))
        return -BFDEV_ENOMEM;

    array->data = data;
    array->capacity = nalloc;

    return -BFDEV_ENOERR;
}

static inline int
array_apply(bfdev_array_t *array, unsigned long count)
{
    if (count <= array->capacity)
        return -BFDEV_ENOERR;

    return array_resize(array, count << 1);
}

static inline void *
array_peek(const bfdev_array_t *array, unsigned long num,
           unsigned long *indexp)
{
    unsigned long index;
    uintptr_t offset;
    bool overflow;

    overflow = bfdev_overflow_check_sub(array->index, num, &index);
    if (bfdev_unlikely(overflow))
        return NULL;

    offset = bfdev_array_offset(array, index);
    if (indexp)
        *indexp = index;

    return array->data + offset;
}

export void *
bfdev_array_push(bfdev_array_t *array, unsigned long num)
{
    unsigned long index, count;
    uintptr_t offset;
    bool overflow;
    int retval;

    overflow = bfdev_overflow_check_add(array->index, num, &count);
    if (bfdev_unlikely(overflow))
        return NULL;

    retval = array_apply(array, count);
    if (bfdev_unlikely(retval))
        return NULL;

    index = array->index;
    array->index = count;
    offset = bfdev_array_offset(array, index);

    return array->data + offset;
}

export void *
bfdev_array_pop(bfdev_array_t *array, unsigned long num)
{
    return array_peek(array, num, &array->index);
}

export void *
bfdev_array_peek(const bfdev_array_t *array, unsigned long num)
{
    return array_peek(array, num, NULL);
}

export int
bfdev_array_resize(bfdev_array_t *array, unsigned long num)
{
    int retval;

    retval = array_apply(array, num);
    if (bfdev_unlikely(retval))
        return retval;

    array->index = num;

    return -BFDEV_ENOERR;
}

export int
bfdev_array_reserve(bfdev_array_t *array, unsigned long num)
{
    unsigned long count;
    bool overflow;

    overflow = bfdev_overflow_check_add(array->index, num, &count);
    if (bfdev_unlikely(overflow))
        return -BFDEV_EOVERFLOW;

    return array_apply(array, count);
}

export void
bfdev_array_release(bfdev_array_t *array)
{
    const bfdev_alloc_t *alloc;

    array->capacity = 0;
    array->index = 0;

    alloc = array->alloc;
    bfdev_free(alloc, array->data);
    array->data = NULL;
}
