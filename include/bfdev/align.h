/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALIGN_H_
#define _BFDEV_ALIGN_H_

#include <bfdev/config.h>
#include <bfdev/stdlib.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_align_check() - Check if an value is aligned.
 * @value: the value being checked for alignment.
 * @align: check alignment size.
 *
 * Alignment must be a power of 2.
 */
#define bfdev_align_check(value, align) ( \
    !((value) & ((align) - 1)) \
)

/**
 * bfdev_align_ptr_check() - Check if an pointer is aligned.
 * @ptr: the pointer being checked for alignment.
 * @align: check alignment size.
 */
#define bfdev_align_ptr_check(ptr, align) ( \
    bfdev_align_check((uintptr_t)(ptr), align) \
)

/**
 * bfdev_align_low() - Return an downward-aligned integer value.
 * @value: value to aligned.
 * @align: alignment size.
 */
#define bfdev_align_low(value, align) ({ \
    (value) & ~((align) - 1); \
})

/**
 * bfdev_align_high() - Return an upward-aligned value.
 * @value: value to aligned.
 * @align: alignment size.
 */
#define bfdev_align_high(value, align) ({ \
    typeof(align) _align = (align); \
    ((value) + (_align - 1)) & ~(_align - 1); \
})

/**
 * bfdev_align_ptr_low() - Return an downward-aligned pointer.
 * @ptr: pointer to aligned.
 * @align: alignment size.
 */
#define bfdev_align_ptr_low(ptr, align) ({ \
    (typeof(ptr))bfdev_align_low((uintptr_t)(ptr), align); \
})

/**
 * bfdev_align_ptr_high() - Return an upward-aligned pointer.
 * @ptr: pointer to aligned.
 * @align: alignment size.
 */
#define bfdev_align_ptr_high(ptr, align) ({ \
    (typeof(ptr))bfdev_align_high((uintptr_t)(ptr), align); \
})

#define bfdev_align_low_adj(value, align) ({ \
    (value) = bfdev_align_low(value, align); \
})

#define bfdev_align_high_adj(value, align) ({ \
    (value) = bfdev_align_high(value, align); \
})

#define bfdev_align_ptr_low_adj(ptr, align) ({ \
    (ptr) = bfdev_align_ptr_low(ptr, align); \
})

#define bfdev_align_ptr_high_adj(ptr, align) ({ \
    (ptr) = bfdev_align_ptr_high(ptr, align); \
})

BFDEV_END_DECLS

#endif /* _BFDEV_ALIGN_H_ */
