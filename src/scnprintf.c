/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/scnprintf.h>
#include <port/stdio.h>
#include <export.h>

export int
bfdev_vscnprintf(char *buf, bfdev_size_t size,
                 const char *fmt, bfdev_va_list args)
{
    int len;

    if (bfdev_unlikely(size == 0))
        return 0;

    len = bfport_vsnprintf(buf, size, fmt, args);
    if (bfdev_unlikely(len < 0))
        return -BFDEV_EINVAL;

    if (bfdev_likely(len < size))
        return len;

    return size - 1;
}

export int
bfdev_scnprintf(char *buf, bfdev_size_t size, const char *fmt, ...)
{
    bfdev_va_list args;
    int len;

    bfdev_va_start(args, fmt);
    len = bfdev_vscnprintf(buf, size, fmt, args);
    bfdev_va_end(args);

    return len;
}
