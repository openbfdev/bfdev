/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/stringhash.h>
#include <export.h>

export unsigned long
bfdev_pjwhash(const char *str)
{
    unsigned long value, hash;

    hash = 0;
    while (*str) {
        hash <<= 4;
        hash += *str++;

        value = hash & (0xfUL << (BFDEV_PJWHASH_BITS - 4));
        if (value) {
            hash ^= value >> (BFDEV_PJWHASH_BITS - 8);
            hash ^= value;
        }
    }

    return hash;
}
