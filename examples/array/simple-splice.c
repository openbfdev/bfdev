/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "array-simple-splice"
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
    void *pmem;

    if (bfdev_array_append_cstr(array, "Hello123X"))
        return 1;

    bfdev_array_seek(array, 4);
    pmem = bfdev_array_splice(array, 1, 3, 5);
    if (!pmem)
        return 1;
    memcpy(pmem, "World", 5);

    bfdev_array_seek(array, 0);
    result = bfdev_array_data(array, 0);
    if (!result)
        return 1;
    bfdev_log_info("%.11s\n", result);

    return 0;
}
