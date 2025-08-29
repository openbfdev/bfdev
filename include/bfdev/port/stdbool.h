/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDBOOL_H_
#define _BFDEV_PORT_STDBOOL_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/* C language standard type */
typedef _Bool bfdev_bool;

enum bfdev_bool {
    bfdev_false = 0,
    bfdev_true = 1,
};

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDBOOL_H_ */
