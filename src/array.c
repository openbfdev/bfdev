/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/array.h>
#include <bfdev/overflow.h>
#include <export.h>

static int
array_resize(bfdev_array_t *array, unsigned long count)
{
    const struct bfdev_alloc *alloc = array->alloc;
    unsigned long nalloc;
    size_t size;
    void *data;

    nalloc = bfdev_max(count << 1, BFDEV_ARRAY_MSIZE);
    size = nalloc * array->cells;

    data = bfdev_realloc(alloc, array->data, size);
    if (bfdev_unlikely(!data))
        return -BFDEV_ENOMEM;

    array->data = data;
    array->capacity = nalloc;

    return -BFDEV_ENOERR;
}

static int
array_apply(bfdev_array_t *array, unsigned long count)
{
    if (count > array->capacity)
        return array_resize(array, count);
    return -BFDEV_ENOERR;
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
    unsigned long index;
    uintptr_t offset;
    bool overflow;

    overflow = bfdev_overflow_check_sub(array->index, num, &index);
    if (bfdev_unlikely(overflow))
        return NULL;

    array->index = index;
    offset = bfdev_array_offset(array, index);

    return array->data + offset;
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
    const struct bfdev_alloc *alloc = array->alloc;

    array->capacity = 0;
    array->index = 0;

    bfdev_free(alloc, array->data);
    array->data = NULL;
}
