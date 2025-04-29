/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "overflow-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stddef.h>
#include <limits.h>
#include <stdbool.h>
#include <bfdev/log.h>
#include <bfdev/overflow.h>
#include <testsuite.h>

#define TEST(calculate, limit) ({ \
    bool cond; \
    cond = bfdev_overflow_##calculate((limit), 1) == (limit); \
    if (!cond) \
        failed = true; \
    cond ? "okay" : "failed"; \
})

TESTSUITE(
    "overflow:selftest", NULL, NULL,
    "overflow selftest"
) {
    bool failed;

    failed = false;
    bfdev_log_info("add char: %s\n", TEST(add, (signed char)SCHAR_MAX));
    bfdev_log_info("add short: %s\n", TEST(add, (signed short)SHRT_MAX));
    bfdev_log_info("add int: %s\n", TEST(add, (signed int)INT_MAX));
    bfdev_log_info("add long: %s\n", TEST(add, (signed long)LONG_MAX));
    bfdev_log_info("add long long: %s\n", TEST(add, (signed long long)LLONG_MAX));

    bfdev_log_info("add unsigned char: %s\n", TEST(add, (unsigned char)UCHAR_MAX));
    bfdev_log_info("add unsigned short: %s\n", TEST(add, (unsigned short)USHRT_MAX));
    bfdev_log_info("add unsigned int: %s\n", TEST(add, (unsigned int)UINT_MAX));
    bfdev_log_info("add unsigned long: %s\n", TEST(add, (unsigned long)ULONG_MAX));
    bfdev_log_info("add unsigned long long: %s\n", TEST(add, (unsigned long long)ULLONG_MAX));

    bfdev_log_info("sub char: %s\n", TEST(sub, (signed char)SCHAR_MIN));
    bfdev_log_info("sub short: %s\n", TEST(sub, (signed short)SHRT_MIN));
    bfdev_log_info("sub int: %s\n", TEST(sub, (signed int)INT_MIN));
    bfdev_log_info("sub long: %s\n", TEST(sub, (signed long)LONG_MIN));
    bfdev_log_info("sub long long: %s\n", TEST(sub, (signed long long)LLONG_MIN));

    bfdev_log_info("sub unsigned char: %s\n", TEST(sub, (unsigned char)0));
    bfdev_log_info("sub unsigned short: %s\n", TEST(sub, (unsigned short)0));
    bfdev_log_info("sub unsigned int: %s\n", TEST(sub, (unsigned int)0));
    bfdev_log_info("sub unsigned long: %s\n", TEST(sub, (unsigned long)0));
    bfdev_log_info("sub unsigned long long: %s\n", TEST(sub, (unsigned long long)0));

    if (failed)
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}
