/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_GUARDS_H_
#define _BFDEV_GUARDS_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

#define BFDEV_CLEAN_TEMPLATE(name, type, free)  \
static inline void                              \
__bfdev_cleanup_##name(void *object)            \
{                                               \
    type _T = *(type *)object;                  \
    free;                                       \
}

#define bfdev_clean_lasting(object) ({  \
    __auto_type __ptr = (object);       \
    (object) = NULL; __ptr;             \
})

#define bfdev_clean(name) \
    __bfdev_cleanup(__bfdev_cleanup_##name)

#define bfdev_clean_return(object) \
    return bfdev_clean_lasting(object)

BFDEV_END_DECLS

#endif /* _BFDEV_GUARDS_H_ */
