/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_RWONCE_H_
#define _BFDEV_ASM_GENERIC_RWONCE_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_READ_ONCE
# define BFDEV_READ_ONCE(x) ({ \
    *(volatile typeof(x) *)&(x); \
})
#endif

#ifndef BFDEV_WRITE_ONCE
# define BFDEV_WRITE_ONCE(x, val) ({ \
    *(volatile typeof(x) *)&(x) = (val); \
})
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_RWONCE_H_ */
