/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_PORT_STDIO_H_
#define _LOCAL_PORT_STDIO_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stdarg.h>

BFDEV_BEGIN_DECLS

extern int
bfport_vsnprintf(char *s, bfdev_size_t maxlen, const char *format,
                 bfdev_va_list arg);

BFDEV_END_DECLS

#endif /* _LOCAL_PORT_STDIO_H_ */
