/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ERRNAME_H_
#define _BFDEV_ERRNAME_H_

#include <bfdev/config.h>
#include <bfdev/errno.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

struct bfdev_errname {
    int errnum;
    const char *name;
    const char *info;
};

extern const struct bfdev_errname bfdev_errname_table[];
extern struct bfdev_errname *bfdev_errname_find(int error);
extern const char *bfdev_errname(int error, const char **infop);

BFDEV_END_DECLS

#endif /* _BFDEV_ERRNAME_H_ */
