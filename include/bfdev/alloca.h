/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALLOCA_H_
#define _BFDEV_ALLOCA_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

#define bfdev_alloca __builtin_alloca
extern __bfdev_malloc void *bfdev_alloca(unsigned int size);

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCA_H_ */
