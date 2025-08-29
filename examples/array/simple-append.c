/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "array-simple-append"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdlib.h>
#include <string.h>
#include <bfdev/log.h>
#include <bfdev/array.h>
#include <bfdev/template/array.h>

int
main(int argc, const char *argv[])
{
    BFDEV_CLASS(bfdev_array, array)(NULL, 1);
    const char *result;

    if (bfdev_array_append_cstr(array, "Hello"))
        return 1;

    if (bfdev_array_append_cstr(array, "World"))
        return 1;

    result = bfdev_array_data(array, 0);
    if (!result)
        return 1;
    bfdev_log_info("%.5s\n", result);

    result = bfdev_array_data(array, 5);
    if (!result)
        return 1;
    bfdev_log_info("%.5s\n", result);

    return 0;
}
