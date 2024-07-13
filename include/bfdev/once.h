/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ONCE_H_
#define _BFDEV_ONCE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

#define BFDEV_DO_ONCE_DONE(condition) ({            \
    static bool __already;                          \
    bool ___cond = !!(condition);                   \
    bool __retval = false;                          \
                                                    \
    if (bfdev_unlikely(___cond && !__already)) {    \
        __already = true;                           \
        __retval = true;                            \
    }                                               \
                                                    \
    bfdev_unlikely(__retval);                       \
})

#define BFDEV_DO_ONCE_ON(condition, func, ...) ({   \
    bool __cond = !!(condition);                    \
    if (BFDEV_DO_ONCE_DONE(__cond))                 \
        func(__VA_ARGS__);                          \
    bfdev_unlikely(__cond);                         \
})

#define BFDEV_DO_ONCE(func, ...) \
    BFDEV_DO_ONCE_ON(true, func, ##__VA_ARGS__)

BFDEV_END_DECLS

#endif /* _BFDEV_ONCE_H_ */
