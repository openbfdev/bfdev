/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SCNPRINTF_H_
#define _BFDEV_SCNPRINTF_H_

#include <bfdev/config.h>
#include <bfdev/stdarg.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

extern __printf(3, 0) int
bfdev_vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

extern __printf(3, 4) int
bfdev_scnprintf(char *buf, size_t size, const char *fmt, ...);

BFDEV_END_DECLS

#endif /* _BFDEV_SCNPRINTF_H_ */
