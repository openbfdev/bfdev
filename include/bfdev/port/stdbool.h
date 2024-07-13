/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDBOOL_H_
#define _BFDEV_PORT_STDBOOL_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/types.h>
#else
# include <stdbool.h>
#endif

BFDEV_BEGIN_DECLS

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDBOOL_H_ */
