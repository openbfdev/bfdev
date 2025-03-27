/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/stdio.h>
#include <export.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <stdio.h>
#endif

hidden int
bfport_vsnprintf(char *s, bfdev_size_t maxlen, const char *fmt,
                 bfdev_va_list arg)
{
    return vsnprintf(s, maxlen, fmt, arg);
}
