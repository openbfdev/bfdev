/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LIMITS_H_
#define _BFDEV_LIMITS_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/stdint.h>
# include <sys/limits.h>
#else
# include <limits.h>
#endif

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
#define BFDEV_UINT8_MAX     ((uint8_t)~0U)
#define BFDEV_UINT16_MAX    ((uint16_t)~0U)
#define BFDEV_UINT32_MAX    ((uint32_t)~0U)
#define BFDEV_UINT64_MAX    ((uint64_t)~0ULL)

/* Minimum of unsigned integral types */
#define BFDEV_UINT8_MIN     ((uint8_t)0U)
#define BFDEV_UINT16_MIN    ((uint16_t)0U)
#define BFDEV_UINT32_MIN    ((uint32_t)0U)
#define BFDEV_UINT64_MIN    ((uint64_t)0ULL)

/* Maximum of signed integral types */
#define BFDEV_INT8_MAX      ((int8_t)(BFDEV_UINT8_MAX >> 1))
#define BFDEV_INT16_MAX     ((int16_t)(BFDEV_UINT16_MAX >> 1))
#define BFDEV_INT32_MAX     ((int32_t)(BFDEV_UINT32_MAX >> 1))
#define BFDEV_INT64_MAX     ((int64_t)(BFDEV_UINT64_MAX >> 1))

/* Minimum of signed integral types */
#define BFDEV_INT8_MIN      ((int8_t)(-BFDEV_INT8_MAX - 1))
#define BFDEV_INT16_MIN     ((int16_t)(-BFDEV_INT16_MAX - 1))
#define BFDEV_INT32_MIN     ((int32_t)(-BFDEV_INT32_MAX - 1))
#define BFDEV_INT64_MIN     ((int64_t)(-BFDEV_INT64_MAX - 1))

/* Maximum of unsigned pointer types */
#define BFDEV_UINTPTR_MAX   ((uintptr_t)~0UL)
#define BFDEV_UINTMAX_MAX   ((uintmax_t)~0ULL)
#define BFDEV_SIZE_MAX      ((size_t)~0UL)

/* Minimum of unsigned pointer types */
#define BFDEV_UINTPTR_MIN   ((uintptr_t)0UL)
#define BFDEV_UINTMAX_MIN   ((uintmax_t)0ULL)
#define BFDEV_SIZE_MIN      ((size_t)0UL)

/* Maximum of signed pointer types */
#define BFDEV_INTPTR_MAX    ((intptr_t)(BFDEV_UINTPTR_MAX >> 1))
#define BFDEV_PTRDIFF_MAX   ((ptrdiff_t)(BFDEV_UINTPTR_MAX >> 1))
#define BFDEV_INTMAX_MAX    ((intmax_t)(BFDEV_UINTMAX_MAX >> 1))
#define BFDEV_SSIZE_MAX     ((ssize_t)(BFDEV_SIZE_MAX >> 1))

/* Minimum of signed pointer types */
#define BFDEV_INTPTR_MIN    ((intptr_t)(-BFDEV_INTPTR_MAX - 1))
#define BFDEV_PTRDIFF_MIN   ((ptrdiff_t)(-BFDEV_PTRDIFF_MAX - 1))
#define BFDEV_INTMAX_MIN    ((intmax_t)(-BFDEV_INTMAX_MAX - 1))
#define BFDEV_SSIZE_MIN     ((ssize_t)(-BFDEV_SSIZE_MAX - 1))

BFDEV_END_DECLS

#endif /* _BFDEV_LIMITS_H_ */
