/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STDINT_H_
#define _BFDEV_STDINT_H_

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/stdint.h>
#else
# include <stdint.h>
#endif

#define BFDEV_BYTES_PER_CHAR        sizeof(char)
#define BFDEV_BYTES_PER_SHORT       sizeof(short)
#define BFDEV_BYTES_PER_INT         sizeof(int)
#define BFDEV_BYTES_PER_LONG        sizeof(long)
#define BFDEV_BYTES_PER_LONG_LONG   sizeof(long long)

#define BFDEV_BYTES_PER_U8          sizeof(uint8_t)
#define BFDEV_BYTES_PER_U16         sizeof(uint16_t)
#define BFDEV_BYTES_PER_U32         sizeof(uint32_t)
#define BFDEV_BYTES_PER_U64         sizeof(uint64_t)
#define BFDEV_BYTES_PER_UINTMAX     sizeof(uintmax_t)
#define BFDEV_BYTES_PER_UINTPTR     sizeof(uintptr_t)

#endif /* _BFDEV_STDINT_H_ */
