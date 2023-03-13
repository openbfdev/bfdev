/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STDDEF_H_
#define _BFDEV_STDDEF_H_

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/stddef.h>
#else
# include <stddef.h>
#endif

#endif /* _BFDEV_STDDEF_H_ */
