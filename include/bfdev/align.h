/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALIGN_H_
#define _BFDEV_ALIGN_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/stdlib.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

/**
 * align_low/high - aligned value.
 * @size: value to aligned.
 * @align: alignment size.
 */
#define bfdev_align_low(size, align) ({ \
    (size) & ~((align) - 1); \
})

#define bfdev_align_high(size, align) ({ \
    typeof(align) _align = (align); \
    ((size) + (_align - 1)) & ~(_align - 1); \
})

#define bfdev_align_ptr_low(ptr, align) ({ \
    (typeof(ptr))bfdev_align_low((uintptr_t)(ptr), align); \
})

#define bfdev_align_ptr_high(ptr, align) ({ \
    (typeof(ptr))bfdev_align_high((uintptr_t)(ptr), align); \
})

#define bfdev_align_check(size, align) ( \
    !((size) & ((align) - 1)) \
)

#define bfdev_align_ptr_check(ptr, align) ( \
    bfdev_align_check((uintptr_t)(ptr), align) \
)

BFDEV_END_DECLS

#endif /* _BFDEV_ALIGN_H_ */
