/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/config.h>

int
main(int argc, const char *argv[])
{
    puts(bfdev_release());
    return 0;
}
