/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ONCE_H_
#define _BFDEV_ONCE_H_

#include <stdbool.h>
#include "compiler.h"

#define DO_ONCE_DONE(condition) ({                  \
    static bool __already;                          \
    bool __cond = !!(condition);                    \
    bool __retval = false;                          \
                                                    \
    if (unlikely(__cond && !__already)) {           \
        __already = true;                           \
        __retval = true;                            \
    }                                               \
                                                    \
    unlikely(__retval);                             \
})

#define DO_ONCE_ON(condition, func, ...) ({         \
    bool __cond = !!(condition);                    \
    if (DO_ONCE_DONE(__cond))                       \
        func(__VA_ARGS__);                          \
    unlikely(__cond);                               \
})

#define DO_ONCE(func, ...) \
    DO_ONCE_ON(true, func, ##__VA_ARGS__)

#endif /* _BFDEV_ONCE_H_ */
