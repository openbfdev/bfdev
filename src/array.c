/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/array.h>
#include <export.h>

export void *
bfdev_array_push(struct bfdev_array *array, unsigned int num)
{
    const struct bfdev_alloc *alloc = array->alloc;
    unsigned int nalloc;
    void *data;

    nalloc = array->index + num;
    if (nalloc > array->capacity) {
        nalloc <<= 1;

        data = bfdev_realloc(alloc, array->data, nalloc * array->cells);
        if (unlikely(!data))
            return NULL;

        array->data = data;
        array->capacity = nalloc;
    }

    data = array->data + array->cells * array->index;
    array->index += num;

    return data;
}

export void *
bfdev_array_pop(struct bfdev_array *array, unsigned int num)
{
    if (!num || array->index < num)
        return NULL;

    array->index -= num;
    return array->data + array->cells * array->index;
}

extern void
bfdev_array_release(struct bfdev_array *array)
{
    const struct bfdev_alloc *alloc = array->alloc;

    array->capacity = 0;
    array->index = 0;

    bfdev_free(alloc, array->data);
    array->data = NULL;
}
