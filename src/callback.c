/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/callback.h>
#include <export.h>

export void
bfdev_cbfunc_noop(void)
{
    /* Nothing */
}

export long
bfdev_cbfunc_ret0(void)
{
    return 0;
}

export long
bfdev_cbfunc_ret1(void)
{
    return 1;
}
