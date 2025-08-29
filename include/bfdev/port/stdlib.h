/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDLIB_H_
#define _BFDEV_PORT_STDLIB_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

extern int
bfdev_rand(void);

extern __bfdev_noreturn void
bfdev_abort(void);

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDLIB_H_ */
