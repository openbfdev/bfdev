/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/config.h>

int
main(int argc, const char *argv[])
{
    printf("arch: %s\n", __bfdev_stringify(BFDEV_ARCH));
    printf("name: %s\n", __bfdev_stringify(BFDEV_NAME));
    printf("version: %s\n", __bfdev_stringify(BFDEV_VERSION));
    printf("commitid: %s\n", __bfdev_stringify(BFDEV_COMMITID));
    printf("branch: %s\n", __bfdev_stringify(BFDEV_BRANCH));
    return 0;
}
