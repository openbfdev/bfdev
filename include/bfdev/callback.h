/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CALLBACK_H_
#define _BFDEV_CALLBACK_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

extern void bfdev_cbfunc_noop(void);
extern long bfdev_cbfunc_ret0(void);
extern long bfdev_cbfunc_ret1(void);

#define bfdev_dummy_noop ((void *)bfdev_cbfunc_noop)
#define bfdev_dummy_ret0 ((void *)bfdev_cbfunc_ret0)
#define bfdev_dummy_ret1 ((void *)bfdev_cbfunc_ret1)

BFDEV_END_DECLS

#endif /* _BFDEV_CALLBACK_H_ */
