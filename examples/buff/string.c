/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "string-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev/buff.h>

int
main(int argc, const char *argv[])
{
    BFDEV_DEFINE_STRING(test, "Hello World!\n");
    fwrite(test.data, test.len, 1, stdout);
    return 0;
}
