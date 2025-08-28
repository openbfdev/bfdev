/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/xprintf.h>
#include <port/stdio.h>
#include <export.h>

/* TODO: PORTME */

__bfdev_weak int
bfport_vsnprintf(char *s, bfdev_size_t maxlen, const char *fmt,
                 bfdev_va_list arg)
{
    return bfdev_xnprintf(s, maxlen, fmt, arg);
}
