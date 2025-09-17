/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_GUARDS_H_
#define _BFDEV_GUARDS_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

/**
 * Guards:
 *
 * The "goto error" pattern is notorious for introducing subtle resource
 * leaks. It is tedious and error prone to add new resource acquisition
 * constraints into code paths that already have several unwind
 * conditions. The "cleanup" helpers enable the compiler to help with
 * this tedium and can aid in maintaining LIFO (last in first out)
 * unwind ordering to avoid unintentional leaks.
 */

#define BFDEV_DEFINE_CLEAN(name, type, free)    \
static inline void                              \
__bfdev_cleanup_##name(void *object)            \
{                                               \
    type _T = *(type *)object;                  \
    free;                                       \
}

#define BFDEV_CLEAN(name) \
    __bfdev_cleanup(__bfdev_cleanup_##name)

#define BFDEV_DEFINE_CLASS(name, type, ctor, dtor, args...) \
typedef type __bfdev_class_##name##_t;                      \
static inline type                                          \
__bfdev_class_##name##_constructor(args)                    \
{                                                           \
    type __tmp = ctor;                                      \
    return __tmp;                                           \
}                                                           \
static inline void                                          \
__bfdev_class_##name##_destructor(type *p)                  \
{                                                           \
    type _T = *p;                                           \
    dtor;                                                   \
}

#define BFDEV_CLASS(name, var) \
    __bfdev_class_##name##_t var \
        __bfdev_cleanup(__bfdev_class_##name##_destructor) = \
        __bfdev_class_##name##_constructor

#define bfdev_taken(object) ({      \
    __auto_type __ptr = (object);   \
    (object) = BFDEV_NULL;          \
    __ptr;                          \
})

BFDEV_END_DECLS

#endif /* _BFDEV_GUARDS_H_ */
