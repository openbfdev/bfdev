/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_UNALIGNED_H_
#define _BFDEV_UNALIGNED_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/attributes.h>
#include <bfdev/asm/unaligned.h>

#ifndef bfdev_unaligned_get_uint
static __always_inline unsigned int
bfdev_unaligned_get_uint(const void *ptr)
{
    return bfdev_arch_unaligned_get_uint(ptr);
}
#endif

#ifndef bfdev_unaligned_get_ulong
static __always_inline unsigned long
bfdev_unaligned_get_ulong(const void *ptr)
{
    return bfdev_arch_unaligned_get_ulong(ptr);
}
#endif

#ifndef bfdev_unaligned_set_uint
static __always_inline void
bfdev_unaligned_set_uint(void *ptr, unsigned int val)
{
    bfdev_arch_unaligned_set_uint(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_ulong
static __always_inline void
bfdev_unaligned_set_ulong(void *ptr, unsigned long val)
{
    bfdev_arch_unaligned_set_ulong(ptr, val);
}
#endif

#ifndef bfdev_unaligned_get_u16
static __always_inline uint16_t
bfdev_unaligned_get_u16(const void *ptr)
{
    return bfdev_arch_unaligned_get_u16(ptr);
}
#endif

#ifndef bfdev_unaligned_get_u32
static __always_inline uint32_t
bfdev_unaligned_get_u32(const void *ptr)
{
    return bfdev_arch_unaligned_get_u32(ptr);
}
#endif

#ifndef bfdev_unaligned_get_u64
static __always_inline uint64_t
bfdev_unaligned_get_u64(const void *ptr)
{
    return bfdev_arch_unaligned_get_u64(ptr);
}
#endif

#ifndef bfdev_unaligned_set_u16
static __always_inline void
bfdev_unaligned_set_u16(void *ptr, uint16_t val)
{
    bfdev_arch_unaligned_set_u16(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_u32
static __always_inline void
bfdev_unaligned_set_u32(void *ptr, uint32_t val)
{
    bfdev_arch_unaligned_set_u32(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_u64
static __always_inline void
bfdev_unaligned_set_u64(void *ptr, uint64_t val)
{
    bfdev_arch_unaligned_set_u64(ptr, val);
}
#endif

#ifndef bfdev_unaligned_get_le16
static __always_inline uint16_t
bfdev_unaligned_get_le16(const void *ptr)
{
    return bfdev_arch_unaligned_get_le16(ptr);
}
#endif

#ifndef bfdev_unaligned_get_le32
static __always_inline uint32_t
bfdev_unaligned_get_le32(const void *ptr)
{
    return bfdev_arch_unaligned_get_le32(ptr);
}
#endif

#ifndef bfdev_unaligned_get_le64
static __always_inline uint64_t
bfdev_unaligned_get_le64(const void *ptr)
{
    return bfdev_arch_unaligned_get_le64(ptr);
}
#endif

#ifndef bfdev_unaligned_set_le16
static __always_inline void
bfdev_unaligned_set_le16(void *ptr, uint16_t val)
{
    bfdev_arch_unaligned_set_le16(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_le32
static __always_inline void
bfdev_unaligned_set_le32(void *ptr, uint32_t val)
{
    bfdev_arch_unaligned_set_le32(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_le64
static __always_inline void
bfdev_unaligned_set_le64(void *ptr, uint64_t val)
{
    bfdev_arch_unaligned_set_le64(ptr, val);
}
#endif

#ifndef bfdev_unaligned_get_be16
static __always_inline uint16_t
bfdev_unaligned_get_be16(const void *ptr)
{
    return bfdev_arch_unaligned_get_be16(ptr);
}
#endif

#ifndef bfdev_unaligned_get_be32
static __always_inline uint32_t
bfdev_unaligned_get_be32(const void *ptr)
{
    return bfdev_arch_unaligned_get_be32(ptr);
}
#endif

#ifndef bfdev_unaligned_get_be64
static __always_inline uint64_t
bfdev_unaligned_get_be64(const void *ptr)
{
    return bfdev_arch_unaligned_get_be64(ptr);
}
#endif

#ifndef bfdev_unaligned_set_be16
static __always_inline void
bfdev_unaligned_set_be16(void *ptr, uint16_t val)
{
    bfdev_arch_unaligned_set_be16(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_be32
static __always_inline void
bfdev_unaligned_set_be32(void *ptr, uint32_t val)
{
    bfdev_arch_unaligned_set_be32(ptr, val);
}
#endif

#ifndef bfdev_unaligned_set_be64
static __always_inline void
bfdev_unaligned_set_be64(void *ptr, uint64_t val)
{
    bfdev_arch_unaligned_set_be64(ptr, val);
}
#endif

#endif /* _BFDEV_UNALIGNED_H_ */
