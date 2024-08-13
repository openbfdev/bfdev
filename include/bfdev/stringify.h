/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STRINGIFY_H_
#define _BFDEV_STRINGIFY_H_

#include <bfdev/cdefs.h>

BFDEV_BEGIN_DECLS

/*
 * Indirect stringification.  Doing two levels allows the parameter to be a
 * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
 * converts to "bar".
 */

#define ___bfdev_stringify(x...) #x
#define __bfdev_stringify(x...) ___bfdev_stringify(x)

BFDEV_END_DECLS

#endif /* _BFDEV_STRINGIFY_H_ */
