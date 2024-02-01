/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_POISON_H_
#define _BFDEV_POISON_H_

#include <bfdev/config.h>
#include <bfdev/const.h>

BFDEV_BEGIN_DECLS

#ifdef BFDEV_POISON_ILLEGAL_ADDRESS
# define BFDEV_POISON_OFFSET BFDEV_AC(BFDEV_POISON_ILLEGAL_ADDRESS, UL)
#else
# define BFDEV_POISON_OFFSET 0
#endif

#define BFDEV_POISON_LIST1      ((void *) (BFDEV_POISON_OFFSET + 0x100))
#define BFDEV_POISON_LIST2      ((void *) (BFDEV_POISON_OFFSET + 0x110))
#define BFDEV_POISON_HLIST1     ((void *) (BFDEV_POISON_OFFSET + 0x120))
#define BFDEV_POISON_HLIST2     ((void *) (BFDEV_POISON_OFFSET + 0x130))
#define BFDEV_POISON_SLIST      ((void *) (BFDEV_POISON_OFFSET + 0x140))

#define BFDEV_POISON_RBNODE1    ((void *) (BFDEV_POISON_OFFSET + 0x400))
#define BFDEV_POISON_RBNODE2    ((void *) (BFDEV_POISON_OFFSET + 0x410))
#define BFDEV_POISON_RBNODE3    ((void *) (BFDEV_POISON_OFFSET + 0x420))

#define BFDEV_POISON_HPNODE1    ((void *) (BFDEV_POISON_OFFSET + 0x500))
#define BFDEV_POISON_HPNODE2    ((void *) (BFDEV_POISON_OFFSET + 0x510))
#define BFDEV_POISON_HPNODE3    ((void *) (BFDEV_POISON_OFFSET + 0x520))

BFDEV_END_DECLS

#endif /* _BFDEV_POISON_H_ */
