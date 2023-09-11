/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdarg.h>
#include <bfdev/once.h>

static void
test_once(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    printf("once test_once: ");
    vprintf(fmt, args);
    va_end(args);
}

static void
test_once_on(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    printf("once test_once_on: ");
    vprintf(fmt, args);
    va_end(args);
}

static void
test_once_done(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    printf("once test_once_done: ");
    vprintf(fmt, args);
    va_end(args);
}

int main(void)
{
    unsigned int count;

    for (count = 0; count < 10; ++count)
        BFDEV_DO_ONCE(test_once, "%d\n", count);

    for (count = 0; count < 10; ++count)
        BFDEV_DO_ONCE_ON(count == 1, test_once_on, "%d\n", count);

    for (count = 0; count < 10; ++count) {
        if (BFDEV_DO_ONCE_DONE(count == 2))
            test_once_done("%d\n", count);
    }

    return 0;
}
