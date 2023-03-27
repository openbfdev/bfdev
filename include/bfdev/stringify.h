/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STRINGIFY_H_
#define _BFDEV_STRINGIFY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Indirect stringification.  Doing two levels allows the parameter to be a
 * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
 * converts to "bar".
 */

#define ___stringify(x...) #x
#define __stringify(x...) ___stringify(x)

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_STRINGIFY_H_ */
