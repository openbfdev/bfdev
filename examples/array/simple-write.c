/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "array-simple-write"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stddef.h>
#include <string.h>
#include <bfdev/log.h>
#include <bfdev/array.h>

int
main(int argc, const char *argv[])
{
    BFDEV_CLASS(bfdev_array, array)(NULL, sizeof(char));
    const char *result;
    void *pmem;

    pmem = bfdev_array_write(array, 5);
    if (!pmem)
        return 1;
    memcpy(pmem, "Hello", 5);

    pmem = bfdev_array_write(array, 5);
    if (!pmem)
        return 1;
    memcpy(pmem, "World", 5);

    bfdev_array_seek(array, 0);
    result = bfdev_array_read(array, 5);
    if (!result)
        return 1;
    bfdev_log_info("%.5s\n", result);

    result = bfdev_array_read(array, 5);
    if (!result)
        return 1;
    bfdev_log_info("%.5s\n", result);

    return 0;
}
