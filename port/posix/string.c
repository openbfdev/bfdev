/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/string.h>
#include <export.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <string.h>
#endif

hidden void *
bfport_memcpy(void *dest, const void *src, bfdev_size_t n)
{
    return memcpy(dest, src, n);
}

hidden void *
bfport_memset(void *s, int c, bfdev_size_t n)
{
    return memset(s, c, n);
}

hidden int
bfport_memcmp(const void *s1, const void *s2, bfdev_size_t n)
{
    return memcmp(s1, s2, n);
}

hidden int
bfport_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

hidden char *
bfport_strchr(const char *s, int c)
{
    return strchr(s, c);
}

hidden bfdev_size_t
bfport_strspn(const char *s, const char *accept)
{
    return strspn(s, accept);
}

hidden bfdev_size_t
bfport_strcspn(const char *s, const char *reject)
{
    return strcspn(s, reject);
}

hidden char *
bfport_strcpy(char *dest, const char *src)
{
    return strcpy(dest, src);
}

hidden char *
bfport_strncpy(char *dest, const char *src, bfdev_size_t n)
{
    return strncpy(dest, src, n);
}

hidden bfdev_size_t
bfport_strlen(const char *s)
{
    return strlen(s);
}

hidden bfdev_size_t
bfport_strnlen(const char *s, bfdev_size_t len)
{
    return strnlen(s, len);
}
