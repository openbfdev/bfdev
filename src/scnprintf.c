/* SPDX-License-Identifier: GPL-2.0-or-later */
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

    len = vsnprintf(buf, size, fmt, args);
    if (bfdev_likely(len < size))
        return len;

    if (size != 0)
        return size - 1;

    return 0;
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
