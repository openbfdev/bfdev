/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_GLOB_H_
#define _BFDEV_GLOB_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

extern bfdev_bool
bfdev_glob(const char *pattern, const char *string);

BFDEV_END_DECLS

#endif /* _BFDEV_GLOB_H_ */
