/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SIZE_H_
#define _BFDEV_SIZE_H_

#include <bfdev/config.h>
#include <bfdev/const.h>

BFDEV_BEGIN_DECLS

#define BFDEV_SZ_1Byte      0x00000001
#define BFDEV_SZ_2Byte      0x00000002
#define BFDEV_SZ_4Byte      0x00000004
#define BFDEV_SZ_8Byte      0x00000008
#define BFDEV_SZ_16Byte     0x00000010
#define BFDEV_SZ_32Byte     0x00000020
#define BFDEV_SZ_64Byte     0x00000040
#define BFDEV_SZ_128Byte    0x00000080
#define BFDEV_SZ_256Byte    0x00000100
#define BFDEV_SZ_512Byte    0x00000200

#define BFDEV_SZ_1KiB       0x00000400
#define BFDEV_SZ_2KiB       0x00000800
#define BFDEV_SZ_4KiB       0x00001000
#define BFDEV_SZ_8KiB       0x00002000
#define BFDEV_SZ_16KiB      0x00004000
#define BFDEV_SZ_32KiB      0x00008000
#define BFDEV_SZ_64KiB      0x00010000
#define BFDEV_SZ_128KiB     0x00020000
#define BFDEV_SZ_256KiB     0x00040000
#define BFDEV_SZ_512KiB     0x00080000

#define BFDEV_SZ_1MiB       0x00100000
#define BFDEV_SZ_2MiB       0x00200000
#define BFDEV_SZ_4MiB       0x00400000
#define BFDEV_SZ_8MiB       0x00800000
#define BFDEV_SZ_16MiB      0x01000000
#define BFDEV_SZ_32MiB      0x02000000
#define BFDEV_SZ_64MiB      0x04000000
#define BFDEV_SZ_128MiB     0x08000000
#define BFDEV_SZ_256MiB     0x10000000
#define BFDEV_SZ_512MiB     0x20000000

#define BFDEV_SZ_1GiB       BFDEV_AC(0x0010000000, ULL)
#define BFDEV_SZ_2GiB       BFDEV_AC(0x0020000000, ULL)
#define BFDEV_SZ_4GiB       BFDEV_AC(0x0040000000, ULL)
#define BFDEV_SZ_8GiB       BFDEV_AC(0x0080000000, ULL)
#define BFDEV_SZ_16GiB      BFDEV_AC(0x0100000000, ULL)
#define BFDEV_SZ_32GiB      BFDEV_AC(0x0200000000, ULL)
#define BFDEV_SZ_64GiB      BFDEV_AC(0x0400000000, ULL)
#define BFDEV_SZ_128GiB     BFDEV_AC(0x0800000000, ULL)
#define BFDEV_SZ_256GiB     BFDEV_AC(0x1000000000, ULL)
#define BFDEV_SZ_512GiB     BFDEV_AC(0x2000000000, ULL)

BFDEV_END_DECLS

#endif /* _BFDEV_SIZE_H_ */
