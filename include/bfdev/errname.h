/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ERRNAME_H_
#define _BFDEV_ERRNAME_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_errname bfdev_errname_t;

struct bfdev_errname {
    int errnum;
    const char *name;
    const char *info;
};

extern const bfdev_errname_t
bfdev_errname_table[];

extern bfdev_errname_t *
bfdev_errname_find(int error);

/**
 * bfdev_errname() - find errno name.
 * @error: the error code to find.
 * @infop: point to info pointer.
 *
 * errname(EINVAL) -> "EINVAL"
 * errname(-EINVAL) -> "-EINVAL"
 */
extern const char *
bfdev_errname(int error, const char **infop);

BFDEV_END_DECLS

#endif /* _BFDEV_ERRNAME_H_ */
