/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/scnprintf.h>
#include <export.h>

export int
bfdev_vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int len;

    if (bfdev_unlikely(size == 0))
        return 0;

    len = bfport_vsnprintf(buf, size, fmt, args);
    if (bfdev_likely(len < size))
        return len;

    return size - 1;
}

export int
bfdev_scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int len;

    va_start(args, fmt);
    len = bfdev_vscnprintf(buf, size, fmt, args);
    va_end(args);

    return len;
}
