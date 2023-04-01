/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALIGN_H_
#define _BFDEV_ALIGN_H_

#include <bfdev/config.h>
#include <bfdev/stdlib.h>
#include <bfdev/stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * align_low/high - aligned value.
 * @size: value to aligned.
 * @align: alignment size.
 */
#define align_low(size, align) ({ \
    (size) & ~((align) - 1); \
})

#define align_high(size, align) ({ \
    typeof(align) _align = (align); \
    ((size) + (_align - 1)) & ~(_align - 1); \
})

#define align_ptr_low(ptr, align) ({ \
    (typeof(ptr))align_low((size_t)(ptr), align); \
})

#define align_ptr_high(ptr, align) ({ \
    (typeof(ptr))align_high((size_t)(ptr), align); \
})

#define align_check(size, align) ( \
    !((size) & ((align) - 1)) \
)

#define align_ptr_check(ptr, align) ( \
    align_check((size_t)(ptr), align) \
)

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ALIGN_H_ */
