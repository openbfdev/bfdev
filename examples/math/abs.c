/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "math-abs"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/log.h>
#include <bfdev/math.h>
#include <bfdev/limits.h>

#define ABS_TEST(name, min, max) ({         \
    long long __result;                     \
    __result = (long long)bfdev_abs(min);   \
    bfdev_log_info(name " %lld -> %lld\n",  \
        (long long)min, __result);          \
    if (max != __result) {                  \
        bfdev_log_err("test failed");       \
        return 1;                           \
    }                                       \
})

int
main(int argc, const char *argv[])
{
    ABS_TEST("char", BFDEV_CHAR_MIN + 1, BFDEV_CHAR_MAX);
    ABS_TEST("short", BFDEV_SHRT_MIN + 1, BFDEV_SHRT_MAX);
    ABS_TEST("int", BFDEV_INT_MIN + 1, BFDEV_INT_MAX);
    ABS_TEST("long", BFDEV_LONG_MIN + 1, BFDEV_LONG_MAX);
    ABS_TEST("long long", BFDEV_LLONG_MIN + 1, BFDEV_LLONG_MAX);

    return 0;
}
