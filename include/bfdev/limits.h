/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LIMITS_H_
#define _BFDEV_LIMITS_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/* Maximum value an 'unsigned char' can hold */
#define BFDEV_UCHAR_MAX     ((unsigned char)~0U)
#define BFDEV_UCHAR_MIN     ((unsigned char)0U)

/* Minimum and maximum values a 'char' can hold */
#define BFDEV_CHAR_MAX      ((signed char)(BFDEV_UCHAR_MAX >> 1))
#define BFDEV_CHAR_MIN      ((signed char)(-BFDEV_CHAR_MAX - 1))

/* Maximum value an 'unsigned short int' can hold */
#define BFDEV_USHRT_MAX     ((unsigned short)~0U)
#define BFDEV_USHRT_MIN     ((unsigned short)0U)

/* Minimum and maximum values a 'signed short int' can hold */
#define BFDEV_SHRT_MAX      ((signed short)(BFDEV_USHRT_MAX >> 1))
#define BFDEV_SHRT_MIN      ((signed short)(-BFDEV_SHRT_MAX - 1))

/* Maximum value an 'unsigned int' can hold */
#define BFDEV_UINT_MAX      ((unsigned int)~0U)
#define BFDEV_UINT_MIN      ((unsigned int)0U)

/* Minimum and maximum values a 'signed int' can hold */
#define BFDEV_INT_MAX       ((signed int)(BFDEV_UINT_MAX >> 1))
#define BFDEV_INT_MIN       ((signed int)(-BFDEV_INT_MAX - 1))

/* Maximum value an 'unsigned long int' can hold */
#define BFDEV_ULONG_MAX     ((unsigned long)~0UL)
#define BFDEV_ULONG_MIN     ((unsigned long)0UL)

/* Minimum and maximum values a 'signed long int' can hold */
#define BFDEV_LONG_MAX      ((signed long)(BFDEV_ULONG_MAX >> 1))
#define BFDEV_LONG_MIN      ((signed long)(-BFDEV_LONG_MAX - 1))

/* Maximum value an 'unsigned long long int' can hold */
#define BFDEV_ULLONG_MAX    ((unsigned long long)~0ULL)
#define BFDEV_ULLONG_MIN    ((unsigned long long)0ULL)

/* Minimum and maximum values a 'signed long long int' can hold */
#define BFDEV_LLONG_MAX     ((signed long long)(BFDEV_ULLONG_MAX >> 1))
#define BFDEV_LLONG_MIN     ((signed long long)(-BFDEV_LLONG_MAX - 1))

/* Maximum of unsigned integral types */
#define BFDEV_UINT8_MAX     ((bfdev_u8)~0U)
#define BFDEV_UINT16_MAX    ((bfdev_u16)~0U)
#define BFDEV_UINT32_MAX    ((bfdev_u32)~0U)
#define BFDEV_UINT64_MAX    ((bfdev_u64)~0ULL)

/* Minimum of unsigned integral types */
#define BFDEV_UINT8_MIN     ((bfdev_u8)0U)
#define BFDEV_UINT16_MIN    ((bfdev_u16)0U)
#define BFDEV_UINT32_MIN    ((bfdev_u32)0U)
#define BFDEV_UINT64_MIN    ((bfdev_u64)0ULL)

/* Maximum of signed integral types */
#define BFDEV_INT8_MAX      ((bfdev_s8)(BFDEV_UINT8_MAX >> 1))
#define BFDEV_INT16_MAX     ((bfdev_s16)(BFDEV_UINT16_MAX >> 1))
#define BFDEV_INT32_MAX     ((bfdev_s32)(BFDEV_UINT32_MAX >> 1))
#define BFDEV_INT64_MAX     ((bfdev_s64)(BFDEV_UINT64_MAX >> 1))

/* Minimum of signed integral types */
#define BFDEV_INT8_MIN      ((bfdev_s8)(-BFDEV_INT8_MAX - 1))
#define BFDEV_INT16_MIN     ((bfdev_s16)(-BFDEV_INT16_MAX - 1))
#define BFDEV_INT32_MIN     ((bfdev_s32)(-BFDEV_INT32_MAX - 1))
#define BFDEV_INT64_MIN     ((bfdev_s64)(-BFDEV_INT64_MAX - 1))

/* Maximum of unsigned pointer types */
#define BFDEV_UINTPTR_MAX   ((bfdev_uintptr_t)~0UL)
#define BFDEV_UINTMAX_MAX   ((bfdev_uintmax_t)~0ULL)
#define BFDEV_SIZE_MAX      ((bfdev_size_t)~0UL)

/* Minimum of unsigned pointer types */
#define BFDEV_UINTPTR_MIN   ((bfdev_uintptr_t)0UL)
#define BFDEV_UINTMAX_MIN   ((uintmax_t)0ULL)
#define BFDEV_SIZE_MIN      ((bfdev_size_t)0UL)

/* Maximum of signed pointer types */
#define BFDEV_INTPTR_MAX    ((bfdev_intptr_t)(BFDEV_UINTPTR_MAX >> 1))
#define BFDEV_PTRDIFF_MAX   ((bfdev_ptrdiff_t)(BFDEV_UINTPTR_MAX >> 1))
#define BFDEV_INTMAX_MAX    ((bfdev_intmax_t)(BFDEV_UINTMAX_MAX >> 1))
#define BFDEV_SSIZE_MAX     ((bfdev_ssize_t)(BFDEV_SIZE_MAX >> 1))

/* Minimum of signed pointer types */
#define BFDEV_INTPTR_MIN    ((bfdev_intptr_t)(-BFDEV_INTPTR_MAX - 1))
#define BFDEV_PTRDIFF_MIN   ((bfdev_ptrdiff_t)(-BFDEV_PTRDIFF_MAX - 1))
#define BFDEV_INTMAX_MIN    ((bfdev_intmax_t)(-BFDEV_INTMAX_MAX - 1))
#define BFDEV_SSIZE_MIN     ((bfdev_ssize_t)(-BFDEV_SSIZE_MAX - 1))

BFDEV_END_DECLS

#endif /* _BFDEV_LIMITS_H_ */
