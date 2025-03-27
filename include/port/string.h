/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_PORT_STRING_H_
#define _LOCAL_PORT_STRING_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

extern void *
bfport_memcpy(void *dest, const void *src, bfdev_size_t n);

extern void *
bfport_memset(void *s, int c, bfdev_size_t n);

extern int
bfport_memcmp(const void *s1, const void *s2, bfdev_size_t n);

extern int
bfport_strcmp(const char *s1, const char *s2);

extern char *
bfport_strchr(const char *s, int c);

extern bfdev_size_t
bfport_strspn(const char *s, const char *accept);

extern bfdev_size_t
bfport_strcspn(const char *s, const char *reject);

extern char *
bfport_strcpy(char *dest, const char *src);

extern char *
bfport_strncpy(char *dest, const char *src, bfdev_size_t n);

extern bfdev_size_t
bfport_strlen(const char *s);

extern bfdev_size_t
bfport_strnlen(const char *s, bfdev_size_t len);

BFDEV_END_DECLS

#endif /* _LOCAL_PORT_STRING_H_ */
