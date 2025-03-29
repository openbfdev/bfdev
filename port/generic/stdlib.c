/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/stdlib.h>
#include <bfdev/prandom.h>
#include <export.h>

/* TODO: PORTME */

static
BFDEV_DEFINE_PRANDOM(generic_rand);

__bfdev_weak __bfdev_malloc void *
bfport_malloc(bfdev_size_t size)
{
    return BFDEV_NULL;
}

__bfdev_weak __bfdev_malloc void *
bfport_calloc(bfdev_size_t nmemb, bfdev_size_t size)
{
    return BFDEV_NULL;
}

__bfdev_weak __bfdev_malloc void *
bfport_realloc(void *ptr, bfdev_size_t size)
{
    return BFDEV_NULL;
}

__bfdev_weak void
bfport_free(void *ptr)
{
    return;
}

__bfdev_weak int
bfport_rand(void)
{
    return bfdev_prandom_value(&generic_rand);
}

__bfdev_weak __bfdev_noreturn void
bfport_abort(void)
{
    for (;;);
}
