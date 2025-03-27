/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#ifndef TYPE
# error "undefine type"
#endif

int
main(int argc, const char *argv[])
{
    const char *name;
    TYPE type;

    name = _Generic(type,
        __signed__ char: "char",
        unsigned char: "char",
        __signed__ short: "short",
        unsigned short: "short",
        __signed__ int: "int",
        unsigned int: "int",
        __signed__ long: "long",
        unsigned long: "long",
        __signed__ long long: "long long",
        unsigned long long: "long long",
        default: NULL
    );

    if (!name)
        return 1;
    printf("%s", name);

    return 0;
}
