/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdint.h>
#include <stddef.h>

#if !defined(TYPE1) || !defined(TYPE2)
# error "Undefine types"
#endif

#define typecheck(type, object) ({ \
    type __dummy1; \
    typeof(object) __dummy2; \
    (void)(&__dummy1 == &__dummy2); \
    1; \
})

int
main(int argc, const char *argv[])
{
    typecheck(TYPE1, (TYPE2){0});
    return 0;
}
