/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "overflow-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/log.h>
#include <bfdev/overflow.h>

int
main(int argc, const char *argv[])
{
    bfdev_log_info("(signed char)127 + 1 = %d\n",
        bfdev_overflow_add((signed char)127, 1));

    bfdev_log_info("(unsigned char)255 + 1 = %d\n",
        bfdev_overflow_add((unsigned char)255, 1));

    bfdev_log_info("(signed char)-128 - 1 = %d\n",
        bfdev_overflow_sub((signed char)-128, 1));

    bfdev_log_info("(unsigned char)0 - 1 = %d\n",
        bfdev_overflow_sub((unsigned char)0, 1));

    return 0;
}
