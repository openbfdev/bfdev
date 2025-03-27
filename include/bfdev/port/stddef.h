/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDDEF_H_
#define _BFDEV_PORT_STDDEF_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_NULL
# define BFDEV_NULL ((void *)0)
#endif

#if __GNUC__ > 3
#define bfdev_offsetof(type, member) \
    __builtin_offsetof(type, member)
#else
#define bfdev_offsetof(type, member) \
    ((unsigned long)((char *)&(((type *)0)->member) - (char *)0))
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDDEF_H_ */
