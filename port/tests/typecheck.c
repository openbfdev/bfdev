/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <stdint.h>
#include <stddef.h>

int
main(int argc, const char *argv[])
{
    bfdev_typecheck(bfdev_s8,  (int8_t){0});
    bfdev_typecheck(bfdev_u8,  (uint8_t){0});
    bfdev_typecheck(bfdev_s16, (int16_t){0});
    bfdev_typecheck(bfdev_u16, (uint16_t){0});
    bfdev_typecheck(bfdev_s32, (int32_t){0});
    bfdev_typecheck(bfdev_u32, (uint32_t){0});
    bfdev_typecheck(bfdev_s64, (int64_t){0});
    bfdev_typecheck(bfdev_u64, (uint64_t){0});

    bfdev_typecheck(bfdev_size_t,    (size_t){0});
    bfdev_typecheck(bfdev_intptr_t,  (intptr_t){0});
    bfdev_typecheck(bfdev_ptrdiff_t, (ptrdiff_t){0});
    bfdev_typecheck(bfdev_intmax_t,  (intmax_t){0});
    bfdev_typecheck(bfdev_uintmax_t, (uintmax_t){0});

    return 0;
}
