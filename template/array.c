/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/template/array.h>
#include <export.h>

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

export int
bfdev_array_append_array(bfdev_array_t *array, const bfdev_array_t *append)
{
    int retval;

    if (array->cells != append->cells)
        return -BFDEV_EPROTO;

    retval = bfdev_array_append(array, append->data, bfdev_array_size(append));
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}

export int
bfdev_array_append_cstr(bfdev_array_t *array, const char *append)
{
    int retval;

    if (array->cells != sizeof(*append))
        return -BFDEV_EPROTO;

    retval = bfdev_array_append(array, append, bfdev_strlen(append));
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}
