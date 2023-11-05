/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_TEXTSEARCH_H_
#define _LOCAL_TEXTSEARCH_H_

#include <bfdev/config.h>
#include <bfdev/textsearch.h>

BFDEV_BEGIN_DECLS

extern int
bfdev_textsearch_register(struct bfdev_ts_algorithm *algo);

extern void
bfdev_textsearch_unregister(struct bfdev_ts_algorithm *algo);

BFDEV_END_DECLS

#endif /* _LOCAL_TEXTSEARCH_H_ */
