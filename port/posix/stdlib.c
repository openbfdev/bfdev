/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <port/stdlib.h>
#include <export.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <stdlib.h>
#endif

__bfdev_malloc void *
bfport_malloc(bfdev_size_t size)
{
    return malloc(size);
}

__bfdev_malloc void *
bfport_calloc(bfdev_size_t nmemb, bfdev_size_t size)
{
    return calloc(nmemb, size);
}

__bfdev_malloc void *
bfport_realloc(void *ptr, bfdev_size_t size)
{
    return realloc(ptr, size);
}

void
bfport_free(void *ptr)
{
    free(ptr);
}

int
bfport_rand(void)
{
    return rand();
}

__bfdev_noreturn void
bfport_abort(void)
{
    abort();
}
