/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SCNPRINTF_H_
#define _BFDEV_SCNPRINTF_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/stdarg.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_vscnprintf - Format a string and place it in a buffer.
 * @buf: The buffer to place the result into.
 * @size: The size of the buffer, including the trailing null space.
 * @fmt: The format string to use.
 * @args: Arguments for the format string.
 *
 * The function returns the number of characters written
 * into @buf. Use vsnprintf() or vscnprintf() in order to avoid
 * buffer overflows.
 */
extern __bfdev_printf(3, 0) int
bfdev_vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

/**
 * bfdev_scnprintf - Format a string and place it in a buffer.
 * @buf: The buffer to place the result into.
 * @size: The size of the buffer, including the trailing null space.
 * @fmt: The format string to use.
 * @...: Arguments for the format string.
 *
 * The function returns the number of characters written
 * into @buf. Use snprintf() or scnprintf() in order to avoid
 * buffer overflows.
 */
extern __bfdev_printf(3, 4) int
bfdev_scnprintf(char *buf, size_t size, const char *fmt, ...);

BFDEV_END_DECLS

#endif /* _BFDEV_SCNPRINTF_H_ */
