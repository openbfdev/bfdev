/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CONTAINER_H_
#define _BFDEV_CONTAINER_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_container_of() - cast a member of a structure out to the containing structure.
 * @ptr: the pointer to the member.
 * @type: the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 */

/* Any const qualifier of @ptr is lost. */
#define bfdev_container_of(ptr, type, member) ({        \
    const typeof(((type *)0)->member) *__mptr = (ptr);  \
    (type *)((char *)__mptr - offsetof(type, member));  \
})

/* If ptr is NULL, ptr is returned unchanged. */
#define bfdev_container_of_safe(ptr, type, member) ({       \
    typeof(ptr) __ptr = (ptr);                              \
    __ptr ? bfdev_container_of(__ptr, type, member) : NULL; \
})

/* Preserve the const-ness of the pointer. */
#define bfdev_container_of_const(ptr, type, member)             \
_Generic(ptr,                                                   \
    const typeof(*(ptr)) *:                                     \
        ((const type *)bfdev_container_of(ptr, type, member)),  \
    default:                                                    \
        ((type *)bfdev_container_of(ptr, type, member))         \
)

BFDEV_END_DECLS

#endif /* _BFDEV_CONTAINER_H_ */
