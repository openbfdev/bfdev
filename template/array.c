/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/template/array.h>
#include <export.h>

export int
bfdev_array_append_array(bfdev_array_t *array, const bfdev_array_t *append)
{
    int retval;

    if (bfdev_unlikely(array->cells != append->cells))
        return -BFDEV_EPROTO;

    retval = bfdev_array_append(array, append->data, bfdev_array_size(append));
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}

export int
bfdev_array_append_buff(bfdev_array_t *array, bfdev_buff_t *append)
{
    int retval;

    if (bfdev_unlikely(array->cells != BFDEV_BYTES_PER_U8))
        return -BFDEV_EPROTO;

    retval = bfdev_array_append(array, append->data, append->len);
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}

export int
bfdev_array_append_cstr(bfdev_array_t *array, const char *append)
{
    int retval;

    if (bfdev_unlikely(array->cells != sizeof(*append)))
        return -BFDEV_EPROTO;

    retval = bfdev_array_append(array, append, bfdev_strlen(append));
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}

export int
bfdev_array_append_char(bfdev_array_t *array, char append)
{
    int retval;

    if (bfdev_unlikely(array->cells != sizeof(append)))
        return -BFDEV_EPROTO;

    retval = bfdev_array_append(array, &append, 1);
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}
