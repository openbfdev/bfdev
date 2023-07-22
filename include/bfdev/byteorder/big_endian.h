/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_
#define _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_

#ifndef __BIG_ENDIAN__
# define __BIG_ENDIAN__
#endif

#include <bfdev/stdint.h>
#include <bfdev/swab.h>

#define bfdev_htons_const(x) ((bfdev_be16)(uint16_t)(x))
#define bfdev_htonl_const(x) ((bfdev_be32)(uint32_t)(x))
#define bfdev_ntohl_const(x) ((uint32_t)(bfdev_be32)(x))
#define bfdev_ntohs_const(x) ((uint16_t)(bfdev_be16)(x))

#define bfdev_cpu_to_le16_const(x) ((bfdev_le16)bfdev_swab16_const((x)))
#define bfdev_cpu_to_le32_const(x) ((bfdev_le32)bfdev_swab32_const((x)))
#define bfdev_cpu_to_le64_const(x) ((bfdev_le64)bfdev_swab64_const((x)))
#define bfdev_le16_to_cpu_const(x) bfdev_swab16_const((uint16_t)(bfdev_le16)(x))
#define bfdev_le32_to_cpu_const(x) bfdev_swab32_const((uint32_t)(bfdev_le32)(x))
#define bfdev_le64_to_cpu_const(x) bfdev_swab64_const((uint64_t)(bfdev_le64)(x))

#define bfdev_cpu_to_be16_const(x) ((bfdev_be16)(uint16_t)(x))
#define bfdev_cpu_to_be32_const(x) ((bfdev_be32)(uint32_t)(x))
#define bfdev_cpu_to_be64_const(x) ((bfdev_be64)(uint64_t)(x))
#define bfdev_be16_to_cpu_const(x) ((uint16_t)(bfdev_be16)(x))
#define bfdev_be32_to_cpu_const(x) ((uint32_t)(bfdev_be32)(x))
#define bfdev_be64_to_cpu_const(x) ((uint64_t)(bfdev_be64)(x))

#define bfdev_cpu_to_le16(x) ((bfdev_le16)bfdev_swab16((x)))
#define bfdev_cpu_to_le32(x) ((bfdev_le32)bfdev_swab32((x)))
#define bfdev_cpu_to_le64(x) ((bfdev_le64)bfdev_swab64((x)))
#define bfdev_le16_to_cpu(x) bfdev_swab16((uint16_t)(bfdev_le16)(x))
#define bfdev_le32_to_cpu(x) bfdev_swab32((uint32_t)(bfdev_le32)(x))
#define bfdev_le64_to_cpu(x) bfdev_swab64((uint64_t)(bfdev_le64)(x))

#define bfdev_be16_to_cpu(x) ((uint16_t)(bfdev_be16)(x))
#define bfdev_be32_to_cpu(x) ((uint32_t)(bfdev_be32)(x))
#define bfdev_be64_to_cpu(x) ((uint64_t)(bfdev_be64)(x))
#define bfdev_cpu_to_be16(x) ((bfdev_be16)(uint16_t)(x))
#define bfdev_cpu_to_be32(x) ((bfdev_be32)(uint32_t)(x))
#define bfdev_cpu_to_be64(x) ((bfdev_be64)(uint64_t)(x))

#define bfdev_cpu_to_le16s(x) bfdev_swab16s((x))
#define bfdev_cpu_to_le32s(x) bfdev_swab32s((x))
#define bfdev_cpu_to_le64s(x) bfdev_swab64s((x))
#define bfdev_le16_to_cpus(x) bfdev_swab16s((x))
#define bfdev_le32_to_cpus(x) bfdev_swab32s((x))
#define bfdev_le64_to_cpus(x) bfdev_swab64s((x))

#define bfdev_cpu_to_be16s(x) do { (void)(x); } while (0)
#define bfdev_cpu_to_be32s(x) do { (void)(x); } while (0)
#define bfdev_cpu_to_be64s(x) do { (void)(x); } while (0)
#define bfdev_be16_to_cpus(x) do { (void)(x); } while (0)
#define bfdev_be32_to_cpus(x) do { (void)(x); } while (0)
#define bfdev_be64_to_cpus(x) do { (void)(x); } while (0)

static __bfdev_always_inline bfdev_le16
bfdev_cpu_to_le16p(const uint16_t *p)
{
    return (bfdev_le16)bfdev_swab16p(p);
}

static __bfdev_always_inline bfdev_le32
bfdev_cpu_to_le32p(const uint32_t *p)
{
    return (bfdev_le32)bfdev_swab32p(p);
}

static __bfdev_always_inline bfdev_le64
bfdev_cpu_to_le64p(const uint64_t *p)
{
    return (bfdev_le64)bfdev_swab64p(p);
}

static __bfdev_always_inline uint16_t
bfdev_le16_to_cpup(const bfdev_le16 *p)
{
    return bfdev_swab16p((uint16_t *)p);
}

static __bfdev_always_inline uint32_t
bfdev_le32_to_cpup(const bfdev_le32 *p)
{
    return bfdev_swab32p((uint32_t *)p);
}

static __bfdev_always_inline uint64_t
bfdev_le64_to_cpup(const bfdev_le64 *p)
{
    return bfdev_swab64p((uint64_t *)p);
}

static __bfdev_always_inline bfdev_be16
bfdev_cpu_to_be16p(const uint16_t *p)
{
    return (bfdev_be16)*p;
}

static __bfdev_always_inline bfdev_be32
bfdev_cpu_to_be32p(const uint32_t *p)
{
    return (bfdev_be32)*p;
}

static __bfdev_always_inline bfdev_be64
bfdev_cpu_to_be64p(const uint64_t *p)
{
    return (bfdev_be64)*p;
}

static __bfdev_always_inline uint16_t
bfdev_be16_to_cpup(const bfdev_be16 *p)
{
    return (uint16_t)*p;
}

static __bfdev_always_inline uint32_t
bfdev_be32_to_cpup(const bfdev_be32 *p)
{
    return (uint32_t)*p;
}

static __bfdev_always_inline uint64_t
bfdev_be64_to_cpup(const bfdev_be64 *p)
{
    return (uint64_t)*p;
}

#endif /* _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_ */
