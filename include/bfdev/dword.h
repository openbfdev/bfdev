/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_DWORD_H_
#define _BFDEV_DWORD_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/asm/dword.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_dword_udiv
# define bfdev_dword_udiv(quot, rem, src, div) \
    bfdev_dword_generic_udiv(quot, rem, src, div)
#endif

#ifndef bfdev_dword_udivd
# define bfdev_dword_udivd(quot, rem, src, div) \
    bfdev_dword_generic_udivd(quot, rem, src, div)
#endif

extern void
bfdev_dword_generic_udiv(bfdev_uw_t *quot, bfdev_uw_t *rem,
                         const bfdev_uw_t *src, bfdev_uw_t div);

extern void
bfdev_dword_generic_udivd(bfdev_uw_t *quot, bfdev_uw_t *rem,
                          const bfdev_uw_t *src, const bfdev_uw_t *div);

BFDEV_END_DECLS

#endif /* _BFDEV_DWORD_H_ */
