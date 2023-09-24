/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TYPECHECK_H_
#define _BFDEV_TYPECHECK_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_typecheck() - Check object type at compile time.
 * @type: types to comparison.
 * @object: the object to check.
 *
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
 */
#define bfdev_typecheck(type, object) ({    \
    type __dummy1;                          \
    typeof(object) __dummy2;                \
    (void)(&__dummy1 == &__dummy2);         \
    1;                                      \
})

/**
 * bfdev_typecheck_func() - Check function type at compile time.
 * @type: types to comparison.
 * @func: the func to check.
 *
 * Check at compile time that 'function' is a certain type, or is a pointer
 * to that type (needs to use typedef for the function type.)
 */
#define bfdev_typecheck_func(type, func) ({ \
    typeof(type) __dummy = func;            \
    (void)__dummy;                          \
    1;                                      \
})

/**
 * bfdev_typecheck_pointer() - Check if object is a pointer at compile time.
 * @pointer: the pointer to check.
 *
 * Check at compile time that something is a pointer type.
 */
#define bfdev_typecheck_pointer(pointer) ({ \
    typeof(pointer) __dummy;                \
    (void)sizeof(*__dummy);                 \
    1;                                      \
})

BFDEV_END_DECLS

#endif /* _BFDEV_TYPECHECK_H_ */
