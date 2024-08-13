/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STRINGHASH_H_
#define _BFDEV_STRINGHASH_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

#define BFDEV_PJWHASH_BITS 32

extern unsigned long
bfdev_pjwhash(const char *str);

BFDEV_END_DECLS

#endif /* _BFDEV_STRINGHASH_H_ */
