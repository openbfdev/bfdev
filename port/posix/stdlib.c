/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/stdlib.h>
#include <export.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <stdlib.h>
#endif

export __bfdev_malloc void *
bfport_malloc(bfdev_size_t size)
{
    return malloc(size);
}

export __bfdev_malloc void *
bfport_calloc(bfdev_size_t nmemb, bfdev_size_t size)
{
    return calloc(nmemb, size);
}

export __bfdev_malloc void *
bfport_realloc(void *ptr, bfdev_size_t size)
{
    return realloc(ptr, size);
}

export void
bfport_free(void *ptr)
{
    free(ptr);
}

export int
bfport_rand(void)
{
    return rand();
}

export __bfdev_noreturn void
bfport_abort(void)
{
    abort();
}
