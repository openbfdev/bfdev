/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LIMITS_H_
#define _BFDEV_LIMITS_H_

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/stdint.h>
# include <sys/limits.h>
#else
# include <limits.h>
#endif

#endif /* _BFDEV_LIMITS_H_ */
