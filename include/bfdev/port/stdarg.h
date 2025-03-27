/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDARG_H_
#define _BFDEV_PORT_STDARG_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

typedef __builtin_va_list bfdev_va_list;
#define bfdev_va_start(v, l) __builtin_va_start(v, l)
#define bfdev_va_arg(v, l) __builtin_va_arg(v, l)
#define bfdev_va_end(v) __builtin_va_end(v)
#define bfdev_va_copy(d, s) __builtin_va_copy(d, s)

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDARG_H_ */
