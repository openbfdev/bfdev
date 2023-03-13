/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STDLIB_H_
#define _BFDEV_STDLIB_H_

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <stdlib.h>
#endif

#endif /* _BFDEV_STDLIB_H_ */
