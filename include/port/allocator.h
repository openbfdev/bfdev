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
    return bfport_malloc(size);
}

static __bfdev_always_inline void *
generic_zalloc(size_t size, void *pdata)
{
    return bfport_calloc(1, size);
}

static __bfdev_always_inline void *
generic_realloc(void *block, size_t resize, void *pdata)
{
    return bfport_realloc(block, resize);
}

static __bfdev_always_inline void
generic_free(void *block, void *pdata)
{
    bfport_free(block);
}

#endif /* _LOCAL_PORT_ALLOCATOR_H_ */
