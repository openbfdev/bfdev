/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/ctype.h>
#include <bfdev/string.h>
#include <export.h>

export __bfdev_weak char *
bfdev_strdiff(const char *addr, int c)
{
    while (*addr) {
        if (*addr != (char)c)
            return (char *)addr;
        addr++;
    }

    return NULL;
}

export __bfdev_weak void *
bfdev_memdiff(const void *addr, int c, size_t size)
{
    const char *p = addr;

    while (size) {
        if (*p != (char)c)
            return (void *)p;
        p++;
        size--;
    }

    return NULL;
}
