/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/xprintf.h>
#include <export.h>

struct vxprintf_buff {
    void *buff;
    bfdev_size_t offset;
    bfdev_size_t size;
};

static void
vxprintf_buff_write(const char *buff, bfdev_size_t len, void *pdata)
{
    struct vxprintf_buff *vxbuff;
    bfdev_uintptr_t offset;

    vxbuff = pdata;
    if (!vxbuff->buff)
        return;

    offset = vxbuff->offset;
    if (bfdev_unlikely(len > vxbuff->size - offset))
        len = vxbuff->size - offset;

    bfdev_memcpy(vxbuff->buff + offset, buff, len);
    vxbuff->offset += len;
}

export bfdev_size_t
bfdev_xnprintf(char *buff, bfdev_size_t size, const char *fmt, bfdev_va_list args)
{
    struct vxprintf_buff vxbuff;
    bfdev_vxprintf_t ctx;
    bfdev_size_t len, end;

    vxbuff.buff = buff;
    vxbuff.size = size - 1;
    vxbuff.offset = 0;

    ctx.write = vxprintf_buff_write;
    ctx.pdata = &vxbuff;

    len = bfdev_vxfprintf(&ctx, fmt, args);
    if (bfdev_unlikely(len == BFDEV_SIZE_MAX))
        return BFDEV_SIZE_MAX;

    if (buff && size) {
        end = bfdev_min(size - 1, len);
        buff[end] = '\0';
    }

    return len;
}

export bfdev_size_t
bfdev_xprintf(char *buff, bfdev_size_t size, const char *fmt, ...)
{
    bfdev_va_list args;
    bfdev_size_t len;

    bfdev_va_start(args, fmt);
    len = bfdev_xnprintf(buff, size, fmt, args);
    bfdev_va_end(args);

    return len;
}
