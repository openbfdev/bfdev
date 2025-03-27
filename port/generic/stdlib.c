/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/stdlib.h>
#include <export.h>

/* TODO: PORTME */

hidden __bfdev_malloc void *
bfport_malloc(bfdev_size_t size)
{
    return BFDEV_NULL;
}

hidden __bfdev_malloc void *
bfport_calloc(bfdev_size_t nmemb, bfdev_size_t size)
{
    return BFDEV_NULL;
}

hidden __bfdev_malloc void *
bfport_realloc(void *ptr, bfdev_size_t size)
{
    return BFDEV_NULL;
}

hidden void
bfport_free(void *ptr)
{
    return;
}

hidden int
bfport_rand(void)
{
    return 0;
}

hidden __bfdev_noreturn void
bfport_abort(void)
{
    for (;;);
}
