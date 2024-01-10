/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BYTEORDER_H_
#define _BFDEV_ASM_GENERIC_BYTEORDER_H_

#include <bfdev/config.h>
#include <endian.h>

#if BYTE_ORDER == LITTLE_ENDIAN
# include <bfdev/byteorder/little-endian.h>
#elif BYTE_ORDER == BIG_ENDIAN
# include <bfdev/byteorder/big-endian.h>
#else
# error "Unknown byte order"
#endif

BFDEV_BEGIN_DECLS

/* Nothing */

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BYTEORDER_H_ */
