/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/string.h>
#include <port/string.h>
#include <export.h>

export void *
bfdev_memcpy(void *dest, const void *src, bfdev_size_t n)
{
    return bfport_memcpy(dest, src, n);
}

export void *
bfdev_memset(void *s, int c, bfdev_size_t n)
{
    return bfport_memset(s, c, n);
}

export int
bfdev_memcmp(const void *s1, const void *s2, bfdev_size_t n)
{
    return bfport_memcmp(s1, s2, n);
}

export int
bfdev_strcmp(const char *s1, const char *s2)
{
    return bfport_strcmp(s1, s2);
}

export char *
bfdev_strchr(const char *s, int c)
{
    return bfport_strchr(s, c);
}

export bfdev_size_t
bfdev_strspn(const char *s, const char *accept)
{
    return bfport_strspn(s, accept);
}

export bfdev_size_t
bfdev_strcspn(const char *s, const char *reject)
{
    return bfport_strcspn(s, reject);
}

export char *
bfdev_strcpy(char *dest, const char *src)
{
    return bfport_strcpy(dest, src);
}

export char *
bfdev_strncpy(char *dest, const char *src, bfdev_size_t n)
{
    return bfport_strncpy(dest, src, n);
}

export bfdev_size_t
bfdev_strlen(const char *s)
{
    return bfport_strlen(s);
}

export bfdev_size_t
bfdev_strnlen(const char *s, bfdev_size_t len)
{
    return bfport_strnlen(s, len);
}
