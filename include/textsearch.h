/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _TEXTSEARCH_H_
#define _TEXTSEARCH_H_

#include <bfdev/config.h>
#include <bfdev/textsearch.h>

BFDEV_BEGIN_DECLS

extern int
bfdev_textsearch_register(struct bfdev_ts_algorithm *algo);

extern int
bfdev_textsearch_unregister(struct bfdev_ts_algorithm *algo);

BFDEV_END_DECLS

#endif /* _TEXTSEARCH_H_ */
