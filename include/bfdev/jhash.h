/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_JHASH_H_
#define _BFDEV_JHASH_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/bitops.h>

BFDEV_BEGIN_DECLS

/* An arbitrary initial parameter */
#define BFDEV_JHASH_INITVAL 0xdeadbeef

/* Mix 3 32-bit values reversibly */
#define bfdev_jhash_mix(a, b, c) {              \
	a -= c; a ^= bfdev_rol32(c,  4); c += b;    \
	b -= a; b ^= bfdev_rol32(a,  6); a += c;    \
	c -= b; c ^= bfdev_rol32(b,  8); b += a;    \
	a -= c; a ^= bfdev_rol32(c, 16); c += b;    \
	b -= a; b ^= bfdev_rol32(a, 19); a += c;    \
	c -= b; c ^= bfdev_rol32(b,  4); b += a;    \
}

/* Final mixing of 3 32-bit values (a,b,c) into c */
#define bfdev_jhash_final(a, b, c) {    \
    c ^= b; c -= bfdev_rol32(b, 14);    \
    a ^= c; a -= bfdev_rol32(c, 11);    \
    b ^= a; b -= bfdev_rol32(a, 25);    \
    c ^= b; c -= bfdev_rol32(b, 16);    \
    a ^= c; a -= bfdev_rol32(c,  4);    \
    b ^= a; b -= bfdev_rol32(a, 14);    \
    c ^= b; c -= bfdev_rol32(b, 24);    \
}

extern uint32_t
bfdev_jhash(const void *data, size_t length, uint32_t initval);

BFDEV_END_DECLS

#endif /* _BFDEV_JHASH_H_ */
