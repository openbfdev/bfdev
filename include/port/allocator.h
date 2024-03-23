/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_PORT_ALLOCATOR_H_
#define _LOCAL_PORT_ALLOCATOR_H_

#include <base.h>
#include <bfdev/allocator.h>

#ifndef __INSIDE_ALLOCATOR__
# error "please don't include this file directly"
#endif

static __bfdev_always_inline void *
generic_alloc(size_t size, void *pdata)
{
    void *retval;

    if (bfdev_alloc_default.alloc) {
        retval = bfdev_alloc_default.alloc(size, pdata);
        return retval;
    }

    return malloc(size);
}

static __bfdev_always_inline void *
generic_zalloc(size_t size, void *pdata)
{
    void *retval;

    if (bfdev_alloc_default.zalloc) {
        retval = bfdev_alloc_default.zalloc(size, pdata);
        return retval;
    }

    return calloc(1, size);
}

static __bfdev_always_inline void *
generic_realloc(void *block, size_t resize, void *pdata)
{
    void *retval;

    if (bfdev_alloc_default.realloc) {
        retval = bfdev_alloc_default.realloc(block, resize, pdata);
        return retval;
    }

    return realloc(block, resize);
}

static __bfdev_always_inline void
generic_free(void *block, void *pdata)
{
    if (bfdev_alloc_default.free) {
        bfdev_alloc_default.free(block, pdata);
        return;
    }

    free((void *)block);
}

#endif /* _LOCAL_PORT_ALLOCATOR_H_ */
