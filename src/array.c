/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/array.h>
#include <export.h>

export void *
bfdev_array_push(struct bfdev_array *array)
{
    const struct bfdev_alloc *alloc;
    void *elt, *new;
    unsigned nalloc;

    alloc = array->alloc;
    if (array->nelts == array->capacity) {
        nalloc = array->capacity << 1;

        new = bfdev_realloc(alloc, array->elts, nalloc * array->cells);
        if (unlikely(!new))
            return NULL;

        array->elts = new;
        array->capacity = nalloc;
    }

    elt = array->elts + array->cells * array->nelts;
    array->nelts++;

    return elt;
}

export void *
bfdev_array_push_num(struct bfdev_array *array, unsigned int num)
{
    const struct bfdev_alloc *alloc;
    void *elt, *new;
    size_t nalloc;

    alloc = array->alloc;
    if (array->nelts == array->capacity) {
        nalloc = bfdev_max(num, array->capacity) << 1;

        new = bfdev_realloc(alloc, array->elts, nalloc * array->cells);
        if (unlikely(!new))
            return NULL;

        array->elts = new;
        array->capacity = nalloc;
    }

    elt = array->elts + array->cells * array->nelts;
    array->nelts += num;

    return elt;
}

extern int
bfdev_array_init(struct bfdev_array *array, const struct bfdev_alloc *alloc,
                 unsigned num, size_t size)
{
    array->elts = bfdev_malloc(alloc, size * num);
    if (unlikely(!array->elts))
        return BFDEV_ENOMEM;

    array->alloc = alloc;
    array->capacity = num;
    array->cells = size;
    array->nelts = 0;

    return BFDEV_ENOERR;
}

extern void
bfdev_array_release(struct bfdev_array *array)
{
    const struct bfdev_alloc *alloc;

    alloc = array->alloc;
    bfdev_free(alloc, array->elts);
}

export struct bfdev_array *
bfdev_array_create(const struct bfdev_alloc *alloc, unsigned int num, size_t size)
{
    struct bfdev_array *array;

    array = bfdev_malloc(alloc, sizeof(*array));
    if (unlikely(!array))
        return NULL;

    if (bfdev_array_init(array, alloc, num, size))
        return NULL;

    return array;
}

export void
bfdev_array_destroy(struct bfdev_array *array)
{
    const struct bfdev_alloc *alloc;

    alloc = array->alloc;
    bfdev_array_release(array);
    bfdev_free(alloc, array);
}
