/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/array.h>
#include <bfdev/overflow.h>
#include <export.h>

export void *
bfdev_array_push(struct bfdev_array *array, unsigned int num)
{
    const struct bfdev_alloc *alloc = array->alloc;
    unsigned int nalloc, index, count;
    bool overflow;
    void *data;

    overflow = bfdev_overflow_check_add(array->index, num, &count);
    if (bfdev_unlikely(overflow))
        return NULL;

    if (count > array->capacity) {
        nalloc = bfdev_max(count << 1, BFDEV_ARRAY_MSIZE);

        data = bfdev_realloc(alloc, array->data, nalloc * array->cells);
        if (bfdev_unlikely(!data))
            return NULL;

        array->data = data;
        array->capacity = nalloc;
    }

    index = array->index;
    array->index = count;

    return bfdev_array_data(array, index);
}

export void *
bfdev_array_pop(struct bfdev_array *array, unsigned int num)
{
    unsigned int index;
    void *data;

    index = array->index - num;
    data = bfdev_array_data(array, index);
    array->index = index;

    return data;
}

export void
bfdev_array_release(struct bfdev_array *array)
{
    const struct bfdev_alloc *alloc = array->alloc;

    array->capacity = 0;
    array->index = 0;

    bfdev_free(alloc, array->data);
    array->data = NULL;
}
