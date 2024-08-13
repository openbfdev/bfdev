/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_UNALIGNED_H_
#define _BFDEV_ASM_GENERIC_UNALIGNED_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/byteorder.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_arch_unaligned_get_type
# define bfdev_arch_unaligned_get_type(type, ptr) ({            \
    const struct { type val; } __bfdev_packed                   \
    *_pptr = (typeof(_pptr))(ptr);                              \
    _pptr->val;                                                 \
})
#endif

#ifndef bfdev_arch_unaligned_set_type
# define bfdev_arch_unaligned_set_type(type, ptr, value) ({     \
    struct { type val; } __bfdev_packed                         \
    *_pptr = (typeof(_pptr))(ptr);                              \
    _pptr->val = (value);                                       \
})
#endif

#ifndef bfdev_arch_unaligned_get
# define bfdev_arch_unaligned_get(ptr) \
    bfdev_arch_unaligned_get_type(typeof(*(ptr)), (ptr))
#endif

#ifndef bfdev_arch_unaligned_set
# define bfdev_arch_unaligned_set(ptr, val) \
    bfdev_arch_unaligned_set_type(typeof(*(ptr)), (ptr), (val))
#endif

#ifndef bfdev_arch_unaligned_get_uint
# define bfdev_arch_unaligned_get_uint bfdev_arch_unaligned_get_uint
static __bfdev_always_inline unsigned int
bfdev_arch_unaligned_get_uint(const void *p)
{
    return bfdev_arch_unaligned_get_type(unsigned int, p);
}
#endif

#ifndef bfdev_arch_unaligned_get_ulong
# define bfdev_arch_unaligned_get_ulong bfdev_arch_unaligned_get_ulong
static __bfdev_always_inline unsigned long
bfdev_arch_unaligned_get_ulong(const void *p)
{
    return bfdev_arch_unaligned_get_type(unsigned long, p);
}
#endif

#ifndef bfdev_arch_unaligned_set_uint
# define bfdev_arch_unaligned_set_uint bfdev_arch_unaligned_set_uint
static __bfdev_always_inline void
bfdev_arch_unaligned_set_uint(void *p, unsigned int val)
{
    bfdev_arch_unaligned_set_type(unsigned int, p, val);
}
#endif

#ifndef bfdev_arch_unaligned_set_ulong
# define bfdev_arch_unaligned_set_ulong bfdev_arch_unaligned_set_ulong
static __bfdev_always_inline void
bfdev_arch_unaligned_set_ulong(void *p, unsigned long val)
{
    bfdev_arch_unaligned_set_type(unsigned long, p, val);
}
#endif

#ifndef bfdev_arch_unaligned_get_u16
# define bfdev_arch_unaligned_get_u16 bfdev_arch_unaligned_get_u16
static __bfdev_always_inline uint16_t
bfdev_arch_unaligned_get_u16(const void *p)
{
    return bfdev_arch_unaligned_get_type(uint16_t, p);
}
#endif

#ifndef bfdev_arch_unaligned_get_u32
# define bfdev_arch_unaligned_get_u32 bfdev_arch_unaligned_get_u32
static __bfdev_always_inline uint32_t
bfdev_arch_unaligned_get_u32(const void *p)
{
    return bfdev_arch_unaligned_get_type(uint32_t, p);
}
#endif

#ifndef bfdev_arch_unaligned_get_u64
# define bfdev_arch_unaligned_get_u64 bfdev_arch_unaligned_get_u64
static __bfdev_always_inline uint64_t
bfdev_arch_unaligned_get_u64(const void *p)
{
    return bfdev_arch_unaligned_get_type(uint64_t, p);
}
#endif

#ifndef bfdev_arch_unaligned_set_u16
# define bfdev_arch_unaligned_set_u16 bfdev_arch_unaligned_set_u16
static __bfdev_always_inline void
bfdev_arch_unaligned_set_u16(void *p, uint16_t val)
{
    bfdev_arch_unaligned_set_type(uint16_t, p, val);
}
#endif

#ifndef bfdev_arch_unaligned_set_u32
# define bfdev_arch_unaligned_set_u32 bfdev_arch_unaligned_set_u32
static __bfdev_always_inline void
bfdev_arch_unaligned_set_u32(void *p, uint32_t val)
{
    bfdev_arch_unaligned_set_type(uint32_t, p, val);
}
#endif

#ifndef bfdev_arch_unaligned_set_u64
# define bfdev_arch_unaligned_set_u64 bfdev_arch_unaligned_set_u64
static __bfdev_always_inline void
bfdev_arch_unaligned_set_u64(void *p, uint64_t val)
{
    bfdev_arch_unaligned_set_type(uint64_t, p, val);
}
#endif

#ifndef bfdev_arch_unaligned_get_le16
# define bfdev_arch_unaligned_get_le16 bfdev_arch_unaligned_get_le16
static __bfdev_always_inline uint16_t
bfdev_arch_unaligned_get_le16(const void *p)
{
    return bfdev_le16_to_cpu(bfdev_arch_unaligned_get_type(bfdev_le16, p));
}
#endif

#ifndef bfdev_arch_unaligned_get_le32
# define bfdev_arch_unaligned_get_le32 bfdev_arch_unaligned_get_le32
static __bfdev_always_inline uint32_t
bfdev_arch_unaligned_get_le32(const void *p)
{
    return bfdev_le32_to_cpu(bfdev_arch_unaligned_get_type(bfdev_le32, p));
}
#endif

#ifndef bfdev_arch_unaligned_get_le64
# define bfdev_arch_unaligned_get_le64 bfdev_arch_unaligned_get_le64
static __bfdev_always_inline uint64_t
bfdev_arch_unaligned_get_le64(const void *p)
{
    return bfdev_le64_to_cpu(bfdev_arch_unaligned_get_type(bfdev_le64, p));
}
#endif

#ifndef bfdev_arch_unaligned_set_le16
# define bfdev_arch_unaligned_set_le16 bfdev_arch_unaligned_set_le16
static __bfdev_always_inline void
bfdev_arch_unaligned_set_le16(void *p, uint16_t val)
{
    bfdev_arch_unaligned_set_type(bfdev_le16, p, bfdev_cpu_to_le16(val));
}
#endif

#ifndef bfdev_arch_unaligned_set_le32
# define bfdev_arch_unaligned_set_le32 bfdev_arch_unaligned_set_le32
static __bfdev_always_inline void
bfdev_arch_unaligned_set_le32(void *p, uint32_t val)
{
    bfdev_arch_unaligned_set_type(bfdev_le32, p, bfdev_cpu_to_le32(val));
}
#endif

#ifndef bfdev_arch_unaligned_set_le64
# define bfdev_arch_unaligned_set_le64 bfdev_arch_unaligned_set_le64
static __bfdev_always_inline void
bfdev_arch_unaligned_set_le64(void *p, uint64_t val)
{
    bfdev_arch_unaligned_set_type(bfdev_le64, p, bfdev_cpu_to_le64(val));
}
#endif

#ifndef bfdev_arch_unaligned_get_be16
# define bfdev_arch_unaligned_get_be16 bfdev_arch_unaligned_get_be16
static __bfdev_always_inline uint16_t
bfdev_arch_unaligned_get_be16(const void *p)
{
    return bfdev_be16_to_cpu(bfdev_arch_unaligned_get_type(bfdev_be16, p));
}
#endif

#ifndef bfdev_arch_unaligned_get_be32
# define bfdev_arch_unaligned_get_be32 bfdev_arch_unaligned_get_be32
static __bfdev_always_inline uint32_t
bfdev_arch_unaligned_get_be32(const void *p)
{
    return bfdev_be32_to_cpu(bfdev_arch_unaligned_get_type(bfdev_be32, p));
}
#endif

#ifndef bfdev_arch_unaligned_get_be64
# define bfdev_arch_unaligned_get_be64 bfdev_arch_unaligned_get_be64
static __bfdev_always_inline uint64_t
bfdev_arch_unaligned_get_be64(const void *p)
{
    return bfdev_be64_to_cpu(bfdev_arch_unaligned_get_type(bfdev_be64, p));
}
#endif

#ifndef bfdev_arch_unaligned_set_be16
# define bfdev_arch_unaligned_set_be16 bfdev_arch_unaligned_set_be16
static __bfdev_always_inline void
bfdev_arch_unaligned_set_be16(void *p, uint16_t val)
{
    bfdev_arch_unaligned_set_type(bfdev_be16, p, bfdev_cpu_to_be16(val));
}
#endif

#ifndef bfdev_arch_unaligned_set_be32
# define bfdev_arch_unaligned_set_be32 bfdev_arch_unaligned_set_be32
static __bfdev_always_inline void
bfdev_arch_unaligned_set_be32(void *p, uint32_t val)
{
    bfdev_arch_unaligned_set_type(bfdev_be32, p, bfdev_cpu_to_be32(val));
}
#endif

#ifndef bfdev_arch_unaligned_set_be64
# define bfdev_arch_unaligned_set_be64 bfdev_arch_unaligned_set_be64
static __bfdev_always_inline void
bfdev_arch_unaligned_set_be64(void *p, uint64_t val)
{
    bfdev_arch_unaligned_set_type(bfdev_be64, p, bfdev_cpu_to_be64(val));
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_UNALIGNED_H_ */
