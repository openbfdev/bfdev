/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/stdlib.h>
#include <port/stdlib.h>
#include <export.h>

export int
bfdev_rand(void)
{
    return bfport_rand();
}

export __bfdev_noreturn void
bfdev_abort(void)
{
    bfport_abort();
}
