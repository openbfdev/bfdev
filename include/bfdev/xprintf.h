/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_XPRINTF_H_
#define _BFDEV_XPRINTF_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/stdarg.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_vxprintf bfdev_vxprintf_t;

typedef void
(*bfdev_vxprintf_write)(const char *buff, bfdev_size_t len, void *pdata);

struct bfdev_vxprintf {
    bfdev_vxprintf_write write;
    void *pdata;
};

extern __bfdev_printf(2, 0) bfdev_size_t
bfdev_vxfprintf(bfdev_vxprintf_t *ctx, const char *fmt, bfdev_va_list args);

extern __bfdev_printf(2, 3) bfdev_size_t
bfdev_xfprintf(bfdev_vxprintf_t *ctx, const char *fmt, ...);

extern __bfdev_printf(3, 0) bfdev_size_t
bfdev_xnprintf(char *buff, bfdev_size_t size, const char *fmt, bfdev_va_list args);

extern __bfdev_printf(3, 4) bfdev_size_t
bfdev_xprintf(char *buff, bfdev_size_t size, const char *fmt, ...);

BFDEV_END_DECLS

#endif /* _BFDEV_XPRINTF_H_ */
