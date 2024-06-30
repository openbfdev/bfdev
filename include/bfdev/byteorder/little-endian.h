/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_
#define _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/swab.h>

BFDEV_BEGIN_DECLS

#ifndef __BFDEV_LITTLE_ENDIAN__
# define __BFDEV_LITTLE_ENDIAN__
#endif

#define bfdev_htons_const(x) ((__bfdev_force bfdev_be16)bfdev_swab16_const((x)))
#define bfdev_htonl_const(x) ((__bfdev_force bfdev_be32)bfdev_swab32_const((x)))
#define bfdev_ntohl_const(x) bfdev_swab32_const((__bfdev_force bfdev_be32)(x))
#define bfdev_ntohs_const(x) bfdev_swab16_const((__bfdev_force bfdev_be16)(x))

#define bfdev_cpu_to_le16_const(x) ((__bfdev_force bfdev_le16)(uint16_t)(x))
#define bfdev_cpu_to_le32_const(x) ((__bfdev_force bfdev_le32)(uint32_t)(x))
#define bfdev_cpu_to_le64_const(x) ((__bfdev_force bfdev_le64)(uint64_t)(x))
#define bfdev_le16_to_cpu_const(x) ((__bfdev_force uint16_t)(bfdev_le16)(x))
#define bfdev_le32_to_cpu_const(x) ((__bfdev_force uint32_t)(bfdev_le32)(x))
#define bfdev_le64_to_cpu_const(x) ((__bfdev_force uint64_t)(bfdev_le64)(x))

#define bfdev_cpu_to_be16_const(x) ((__bfdev_force bfdev_be16)bfdev_swab16_const((x)))
#define bfdev_cpu_to_be32_const(x) ((__bfdev_force bfdev_be32)bfdev_swab32_const((x)))
#define bfdev_cpu_to_be64_const(x) ((__bfdev_force bfdev_be64)bfdev_swab64_const((x)))
#define bfdev_be16_to_cpu_const(x) bfdev_swab16_const((__bfdev_force uint16_t)(bfdev_be16)(x))
#define bfdev_be32_to_cpu_const(x) bfdev_swab32_const((__bfdev_force uint32_t)(bfdev_be32)(x))
#define bfdev_be64_to_cpu_const(x) bfdev_swab64_const((__bfdev_force uint64_t)(bfdev_be64)(x))

#define bfdev_cpu_to_le16(x) ((__bfdev_force bfdev_le16)(uint16_t)(x))
#define bfdev_cpu_to_le32(x) ((__bfdev_force bfdev_le32)(uint32_t)(x))
#define bfdev_cpu_to_le64(x) ((__bfdev_force bfdev_le64)(uint64_t)(x))
#define bfdev_le16_to_cpu(x) ((__bfdev_force uint16_t)(bfdev_le16)(x))
#define bfdev_le32_to_cpu(x) ((__bfdev_force uint32_t)(bfdev_le32)(x))
#define bfdev_le64_to_cpu(x) ((__bfdev_force uint64_t)(bfdev_le64)(x))

#define bfdev_cpu_to_be16(x) ((__bfdev_force bfdev_be16)bfdev_swab16((x)))
#define bfdev_cpu_to_be32(x) ((__bfdev_force bfdev_be32)bfdev_swab32((x)))
#define bfdev_cpu_to_be64(x) ((__bfdev_force bfdev_be64)bfdev_swab64((x)))
#define bfdev_be16_to_cpu(x) bfdev_swab16((__bfdev_force uint16_t)(bfdev_be16)(x))
#define bfdev_be32_to_cpu(x) bfdev_swab32((__bfdev_force uint32_t)(bfdev_be32)(x))
#define bfdev_be64_to_cpu(x) bfdev_swab64((__bfdev_force uint64_t)(bfdev_be64)(x))

#define bfdev_cpu_to_le16s(x) do { (void)(x); } while (0)
#define bfdev_cpu_to_le32s(x) do { (void)(x); } while (0)
#define bfdev_cpu_to_le64s(x) do { (void)(x); } while (0)
#define bfdev_le16_to_cpus(x) do { (void)(x); } while (0)
#define bfdev_le32_to_cpus(x) do { (void)(x); } while (0)
#define bfdev_le64_to_cpus(x) do { (void)(x); } while (0)

#define bfdev_cpu_to_be16s(x) bfdev_swab16s((x))
#define bfdev_cpu_to_be32s(x) bfdev_swab32s((x))
#define bfdev_cpu_to_be64s(x) bfdev_swab64s((x))
#define bfdev_be16_to_cpus(x) bfdev_swab16s((x))
#define bfdev_be32_to_cpus(x) bfdev_swab32s((x))
#define bfdev_be64_to_cpus(x) bfdev_swab64s((x))

static __bfdev_always_inline bfdev_le16
bfdev_cpu_to_le16p(const uint16_t *p)
{
    return (__bfdev_force bfdev_le16)*p;
}

static __bfdev_always_inline bfdev_le32
bfdev_cpu_to_le32p(const uint32_t *p)
{
    return (__bfdev_force bfdev_le32)*p;
}

static __bfdev_always_inline bfdev_le64
bfdev_cpu_to_le64p(const uint64_t *p)
{
    return (__bfdev_force bfdev_le64)*p;
}

static __bfdev_always_inline uint16_t
bfdev_le16_to_cpup(const bfdev_le16 *p)
{
    return (__bfdev_force uint16_t)*p;
}

static __bfdev_always_inline uint32_t
bfdev_le32_to_cpup(const bfdev_le32 *p)
{
    return (__bfdev_force uint32_t)*p;
}

static __bfdev_always_inline uint64_t
bfdev_le64_to_cpup(const bfdev_le64 *p)
{
    return (__bfdev_force uint64_t)*p;
}

static __bfdev_always_inline bfdev_be16
bfdev_cpu_to_be16p(const uint16_t *p)
{
    return (__bfdev_force bfdev_be16)bfdev_swab16p(p);
}

static __bfdev_always_inline bfdev_be32
bfdev_cpu_to_be32p(const uint32_t *p)
{
    return (__bfdev_force bfdev_be32)bfdev_swab32p(p);
}

static __bfdev_always_inline bfdev_be64
bfdev_cpu_to_be64p(const uint64_t *p)
{
    return (__bfdev_force bfdev_be64)bfdev_swab64p(p);
}

static __bfdev_always_inline uint16_t
bfdev_be16_to_cpup(const bfdev_be16 *p)
{
    return bfdev_swab16p((__bfdev_force uint16_t *)p);
}

static __bfdev_always_inline uint32_t
bfdev_be32_to_cpup(const bfdev_be32 *p)
{
    return bfdev_swab32p((__bfdev_force uint32_t *)p);
}

static __bfdev_always_inline uint64_t
bfdev_be64_to_cpup(const bfdev_be64 *p)
{
    return bfdev_swab64p((__bfdev_force uint64_t *)p);
}

BFDEV_END_DECLS

#endif /* _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_ */
