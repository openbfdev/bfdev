/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_
#define _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_

#ifndef __LITTLE_ENDIAN__
# define __LITTLE_ENDIAN__
#endif

#include <bfdev/stdint.h>
#include <bfdev/swab.h>

#define bfdev_htons_constant(x) ((bfdev_be16)bfdev_constant_swab16((x)))
#define bfdev_htonl_constant(x) ((bfdev_be32)bfdev_constant_swab32((x)))
#define bfdev_ntohl_constant(x) bfdev_constant_swab32((bfdev_be32)(x))
#define bfdev_ntohs_constant(x) bfdev_constant_swab16((bfdev_be16)(x))

#define bfdev_cpu_to_le16_constant(x) ((bfdev_le16)(uint16_t)(x))
#define bfdev_cpu_to_le32_constant(x) ((bfdev_le32)(uint32_t)(x))
#define bfdev_cpu_to_le64_constant(x) ((bfdev_le64)(uint64_t)(x))
#define bfdev_le16_to_cpu_constant(x) ((uint16_t)(bfdev_le16)(x))
#define bfdev_le32_to_cpu_constant(x) ((uint32_t)(bfdev_le32)(x))
#define bfdev_le64_to_cpu_constant(x) ((uint64_t)(bfdev_le64)(x))

#define bfdev_cpu_to_be16_constant(x) ((bfdev_be16)bfdev_constant_swab16((x)))
#define bfdev_cpu_to_be32_constant(x) ((bfdev_be32)bfdev_constant_swab32((x)))
#define bfdev_cpu_to_be64_constant(x) ((bfdev_be64)bfdev_constant_swab64((x)))
#define bfdev_be16_to_cpu_constant(x) bfdev_constant_swab16((uint16_t)(bfdev_be16)(x))
#define bfdev_be32_to_cpu_constant(x) bfdev_constant_swab32((uint32_t)(bfdev_be32)(x))
#define bfdev_be64_to_cpu_constant(x) bfdev_constant_swab64((uint64_t)(bfdev_be64)(x))

#define bfdev_cpu_to_le16(x) ((bfdev_le16)(uint16_t)(x))
#define bfdev_cpu_to_le32(x) ((bfdev_le32)(uint32_t)(x))
#define bfdev_cpu_to_le64(x) ((bfdev_le64)(uint64_t)(x))
#define bfdev_le16_to_cpu(x) ((uint16_t)(bfdev_le16)(x))
#define bfdev_le32_to_cpu(x) ((uint32_t)(bfdev_le32)(x))
#define bfdev_le64_to_cpu(x) ((uint64_t)(bfdev_le64)(x))

#define bfdev_cpu_to_be16(x) ((bfdev_be16)bfdev_swab16((x)))
#define bfdev_cpu_to_be32(x) ((bfdev_be32)bfdev_swab32((x)))
#define bfdev_cpu_to_be64(x) ((bfdev_be64)bfdev_swab64((x)))
#define bfdev_be16_to_cpu(x) bfdev_swab16((uint16_t)(bfdev_be16)(x))
#define bfdev_be32_to_cpu(x) bfdev_swab32((uint32_t)(bfdev_be32)(x))
#define bfdev_be64_to_cpu(x) bfdev_swab64((uint64_t)(bfdev_be64)(x))

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

static __always_inline bfdev_le16
bfdev_cpu_to_le16p(const uint16_t *p)
{
    return (bfdev_le16)*p;
}

static __always_inline bfdev_le32
bfdev_cpu_to_le32p(const uint32_t *p)
{
    return (bfdev_le32)*p;
}

static __always_inline bfdev_le64
bfdev_cpu_to_le64p(const uint64_t *p)
{
    return (bfdev_le64)*p;
}

static __always_inline uint16_t
bfdev_le16_to_cpup(const bfdev_le16 *p)
{
    return (uint16_t)*p;
}

static __always_inline uint32_t
bfdev_le32_to_cpup(const bfdev_le32 *p)
{
    return (uint32_t)*p;
}

static __always_inline uint64_t
bfdev_le64_to_cpup(const bfdev_le64 *p)
{
    return (uint64_t)*p;
}

static __always_inline bfdev_be16
bfdev_cpu_to_be16p(const uint16_t *p)
{
    return (bfdev_be16)bfdev_swab16p(p);
}

static __always_inline bfdev_be32
bfdev_cpu_to_be32p(const uint32_t *p)
{
    return (bfdev_be32)bfdev_swab32p(p);
}

static __always_inline bfdev_be64
bfdev_cpu_to_be64p(const uint64_t *p)
{
    return (bfdev_be64)bfdev_swab64p(p);
}

static __always_inline uint16_t
bfdev_be16_to_cpup(const bfdev_be16 *p)
{
    return bfdev_swab16p((uint16_t *)p);
}

static __always_inline uint32_t
bfdev_be32_to_cpup(const bfdev_be32 *p)
{
    return bfdev_swab32p((uint32_t *)p);
}

static __always_inline uint64_t
bfdev_be64_to_cpup(const bfdev_be64 *p)
{
    return bfdev_swab64p((uint64_t *)p);
}

#endif /* _BFDEV_BYTEORDER_LITTLE_ENDIAN_H_ */
