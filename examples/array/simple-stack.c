/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "array-simple-stack"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdlib.h>
#include <string.h>
#include <bfdev/log.h>
#include <bfdev/array.h>

int
main(int argc, const char *argv[])
{
    BFDEV_CLASS(bfdev_array, array)(NULL, 1);
    const char *result;
    void *pmem;

    pmem = bfdev_array_push(array, 5);
    if (!pmem)
        return 1;
    memcpy(pmem, "World", 5);

    pmem = bfdev_array_push(array, 5);
    if (!pmem)
        return 1;
    memcpy(pmem, "Hello", 5);

    result = bfdev_array_pop(array, 5);
    if (!result)
        return 1;
    bfdev_log_info("%.5s\n", result);

    result = bfdev_array_pop(array, 5);
    if (!result)
        return 1;
    bfdev_log_info("%.5s\n", result);

    return 0;
}
