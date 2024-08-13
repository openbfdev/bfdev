/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDINT_H_
#define _BFDEV_PORT_STDINT_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/stdint.h>
#else
# include <stdint.h>
#endif

BFDEV_BEGIN_DECLS

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDINT_H_ */
