/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/guards.h>

static const char *
test_ctor(void)
{
    return strdup("helloworld");
}

static void
test_dtor(const char *obj)
{
    free((void *)obj);
}

BFDEV_DEFINE_CLASS(test, const char *,
    test_ctor(),
    test_dtor(_T)
)

int
main(int argc, const char *argv[])
{
    BFDEV_CLASS(test, str)();

    if (!str)
        return 1;
    printf("%s\n", str);

    return 0;
}
