/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ERRNAME_H_
#define _BFDEV_ERRNAME_H_

#include <bfdev/config.h>
#include <bfdev/errno.h>
#include <bfdev/stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct errname_entry {
    int errnum;
    const char *name;
    const char *info;
};

extern const struct errname_entry bfdev_errname_table[];
extern struct errname_entry *bfdev_errname_find(int error);
extern const char *bfdev_errname(int error, const char **infop);

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ERRNAME_H_ */
