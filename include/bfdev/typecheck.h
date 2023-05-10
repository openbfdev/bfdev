/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TYPECHECK_H_
#define _BFDEV_TYPECHECK_H_

#include <bfdev/config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bfdev_typecheck(type, object) ({    \
    type __dummy1;                          \
    typeof(object) __dummy2;                \
    (void)(&__dummy1 == &__dummy2);         \
    1;                                      \
})

#define bfdev_typecheck_func(type, func) ({ \
    typeof(type) __dummy = func;            \
    (void)__dummy;                          \
    1;                                      \
})

#define bfdev_typecheck_pointer(pointer) ({ \
    typeof(pointer) __dummy;                \
    (void)sizeof(*__dummy);                 \
    1;                                      \
})

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_TYPECHECK_H_ */
