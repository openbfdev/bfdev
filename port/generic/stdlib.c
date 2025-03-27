/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/stdlib.h>
#include <export.h>

/* TODO: PORTME */

export __bfdev_malloc void *
bfport_malloc(bfdev_size_t size)
{
    return BFDEV_NULL;
}

export __bfdev_malloc void *
bfport_calloc(bfdev_size_t nmemb, bfdev_size_t size)
{
    return BFDEV_NULL;
}

export __bfdev_malloc void *
bfport_realloc(void *ptr, bfdev_size_t size)
{
    return BFDEV_NULL;
}

export void
bfport_free(void *ptr)
{
    return;
}

export int
bfport_rand(void)
{
    return 0;
}

export __bfdev_noreturn void
bfport_abort(void)
{
    for (;;);
}
