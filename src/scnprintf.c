/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/scnprintf.h>
#include <export.h>

/**
 * vscnprintf - format a string and place it in a buffer.
 * @buf: The buffer to place the result into.
 * @size: The size of the buffer, including the trailing null space.
 * @fmt: The format string to use.
 * @...: Arguments for the format string.
 */
export int
vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int len;

    len = vsnprintf(buf, size, fmt, args);
    if (likely(len < size))
        return len;

    if (size != 0)
        return size - 1;

    return 0;
}

/**
 * scnprintf - format a string and place it in a buffer.
 * @buf: The buffer to place the result into.
 * @size: The size of the buffer, including the trailing null space.
 * @fmt: The format string to use.
 * @...: Arguments for the format string.
 */
export int
scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int len;

    va_start(args, fmt);
    len = vscnprintf(buf, size, fmt, args);
    va_end(args);

    return len;
}
