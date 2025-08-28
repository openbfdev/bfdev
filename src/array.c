/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/array.h>
#include <bfdev/overflow.h>
#include <export.h>

static inline bfdev_size_t
array_reqsize(bfdev_array_t *array, unsigned long count)
{
    unsigned long request;
    bfdev_size_t size;

    request = bfdev_max(BFDEV_ARRAY_MINSIZE, count);
    size = bfdev_pow2_roundup(request * array->cells);

    return size;
}

static inline int
array_resize(bfdev_array_t *array, bfdev_size_t size)
{
    const bfdev_alloc_t *alloc;
    void *data;

    alloc = array->alloc;
    data = bfdev_realloc(alloc, array->data, size);
    if (bfdev_unlikely(!data))
        return -BFDEV_ENOMEM;

    array->data = data;
    array->capacity = size / array->cells;

    return -BFDEV_ENOERR;
}

static inline int
array_apply(bfdev_array_t *array, unsigned long count)
{
    bfdev_size_t reqsize;

    if (count <= array->capacity)
        return -BFDEV_ENOERR;

    reqsize = array_reqsize(array, count);
    BFDEV_BUG_ON(count * array->cells > reqsize);

    return array_resize(array, reqsize);
}

static inline void *
array_peek(const bfdev_array_t *array, unsigned long num, unsigned long *idxp)
{
    unsigned long index;
    bfdev_uintptr_t offset;
    bfdev_bool overflow;

    overflow = bfdev_overflow_check_sub(array->index, num, &index);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    offset = bfdev_array_offset(array, index);
    if (idxp)
        *idxp = index;

    return array->data + offset;
}

static inline void *
array_consume(bfdev_array_t *array, unsigned long num, bfdev_bool extend)
{
    unsigned long end;
    bfdev_bool overflow;
    void *data;
    int retval;

    overflow = bfdev_overflow_check_add(array->seek, num, &end);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    if (end > array->index) {
        if (bfdev_unlikely(!extend))
            return BFDEV_NULL;

        retval = array_apply(array, end);
        if (bfdev_unlikely(retval))
            return BFDEV_NULL;
        array->index = end;
    }

    data = array->data + bfdev_array_offset(array, array->seek);
    array->seek = end;

    return data;
}

export void *
bfdev_array_push(bfdev_array_t *array, unsigned long num)
{
    unsigned long index, count;
    bfdev_uintptr_t offset;
    bfdev_bool overflow;
    int retval;

    overflow = bfdev_overflow_check_add(array->index, num, &count);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    retval = array_apply(array, count);
    if (bfdev_unlikely(retval))
        return BFDEV_NULL;

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
    return array_peek(array, num, BFDEV_NULL);
}

export void *
bfdev_array_data(const bfdev_array_t *array, unsigned long index)
{
    unsigned long actual;
    bfdev_bool overflow;

    overflow = bfdev_overflow_check_add(array->seek, index, &actual);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    if (bfdev_unlikely(actual >= array->index))
        return BFDEV_NULL;

    return array->data + bfdev_array_offset(array, actual);
}

export const void *
bfdev_array_read(bfdev_array_t *array, unsigned long num)
{
    return array_consume(array, num, bfdev_false);
}

export void *
bfdev_array_write(bfdev_array_t *array, unsigned long num)
{
    return array_consume(array, num, bfdev_true);
}

export int
bfdev_array_append(bfdev_array_t *array, const void *data, unsigned long num)
{
    bfdev_size_t size;
    void *buff;

    buff = bfdev_array_push(array, num);
    if (bfdev_unlikely(!buff))
        return -BFDEV_ENOMEM;

    size = bfdev_array_offset(array, num);
    bfdev_memcpy(buff, data, size);

    return -BFDEV_ENOERR;
}

export void *
bfdev_array_splice(bfdev_array_t *array, unsigned long index,
                   unsigned long delnum, unsigned long newnum)
{
    unsigned long actual, delcut, newcut, count;
    void *start, *end;
    bfdev_size_t behind;
    bfdev_bool overflow;
    int retval;

    overflow = bfdev_overflow_check_add(array->seek, index, &actual);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    if (bfdev_unlikely(actual >= array->index))
        return BFDEV_NULL;

    overflow = bfdev_overflow_check_add(actual, delnum, &delcut);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    if (bfdev_unlikely(delcut > array->index))
        return BFDEV_NULL;

    overflow = bfdev_overflow_check_add(actual, newnum, &newcut);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    overflow = bfdev_overflow_check_add(array->index, newnum, &count) ||
               bfdev_overflow_check_sub(count, delnum, &count);
    if (bfdev_unlikely(overflow))
        return BFDEV_NULL;

    behind = bfdev_array_offset(array, array->index - delcut);
    array->index = count;

    retval = array_apply(array, count);
    if (bfdev_unlikely(retval))
        return BFDEV_NULL;

    start = array->data + bfdev_array_offset(array, delcut);
    end = array->data + bfdev_array_offset(array, newcut);
    bfdev_memmove(end, start, behind);

    return array->data + bfdev_array_offset(array, actual);
}

export int
bfdev_array_reserve(bfdev_array_t *array, unsigned long num)
{
    unsigned long count;
    bfdev_bool overflow;

    overflow = bfdev_overflow_check_add(array->index, num, &count);
    if (bfdev_unlikely(overflow))
        return -BFDEV_EOVERFLOW;

    return array_apply(array, count);
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

export void
bfdev_array_release(bfdev_array_t *array)
{
    const bfdev_alloc_t *alloc;

    bfdev_array_reset(array);
    array->capacity = 0;

    alloc = array->alloc;
    bfdev_free(alloc, array->data);
    array->data = BFDEV_NULL;
}
