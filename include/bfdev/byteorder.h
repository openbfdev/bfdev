/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_BYTEORDER_H_
#define _BFDEV_BYTEORDER_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/bits.h>
#include <bfdev/asm/byteorder.h>

#define _bfdev_cpu_to_le_type(type)     bfdev_cpu_to_le##type
#define bfdev_cpu_to_le_type(type)      _bfdev_cpu_to_le_type(type)
#define _bfdev_cpu_to_be_type(type)     bfdev_cpu_to_be##type
#define bfdev_cpu_to_be_type(type)      _bfdev_cpu_to_be_type(type)
#define _bfdev_le_type_to_cpu(type)     bfdev_le##type##_to_cpu
#define bfdev_le_type_to_cpu(type)      _bfdev_le_type_to_cpu(type)
#define _bfdev_be_type_to_cpu(type)     bfdev_be##type##_to_cpu
#define bfdev_be_type_to_cpu(type)      _bfdev_be_type_to_cpu(type)

#define bfdev_cpu_to_le_short(val)      bfdev_cpu_to_le_type(BFDEV_BITS_PER_SHORT)(val)
#define bfdev_cpu_to_le_int(val)        bfdev_cpu_to_le_type(BFDEV_BITS_PER_INT)(val)
#define bfdev_cpu_to_le_long(val)       bfdev_cpu_to_le_type(BFDEV_BITS_PER_LONG)(val)
#define bfdev_cpu_to_le_long_long(val)  bfdev_cpu_to_le_type(BFDEV_BITS_PER_LONG_LONG)(val)

#define bfdev_cpu_to_be_short(val)      bfdev_cpu_to_be_type(BFDEV_BITS_PER_SHORT)(val)
#define bfdev_cpu_to_be_int(val)        bfdev_cpu_to_be_type(BFDEV_BITS_PER_INT)(val)
#define bfdev_cpu_to_be_long(val)       bfdev_cpu_to_be_type(BFDEV_BITS_PER_LONG)(val)
#define bfdev_cpu_to_be_long_long(val)  bfdev_cpu_to_be_type(BFDEV_BITS_PER_LONG_LONG)(val)

#define bfdev_le_short_to_cpu(val)      bfdev_le_type_to_cpu(BFDEV_BITS_PER_SHORT)(val)
#define bfdev_le_int_to_cpu(val)        bfdev_le_type_to_cpu(BFDEV_BITS_PER_INT)(val)
#define bfdev_le_long_to_cpu(val)       bfdev_le_type_to_cpu(BFDEV_BITS_PER_LONG)(val)
#define bfdev_le_long_long_to_cpu(val)  bfdev_le_type_to_cpu(BFDEV_BITS_PER_LONG_LONG)(val)

#define bfdev_be_short_to_cpu(val)      bfdev_be_type_to_cpu(BFDEV_BITS_PER_SHORT)(val)
#define bfdev_be_int_to_cpu(val)        bfdev_be_type_to_cpu(BFDEV_BITS_PER_INT)(val)
#define bfdev_be_long_to_cpu(val)       bfdev_be_type_to_cpu(BFDEV_BITS_PER_LONG)(val)
#define bfdev_be_long_long_to_cpu(val)  bfdev_be_type_to_cpu(BFDEV_BITS_PER_LONG_LONG)(val)

#endif /* _BFDEV_BYTEORDER_H_ */
