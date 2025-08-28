/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <port/string.h>
#include <export.h>

__bfdev_weak void *
bfport_memcpy(void *dest, const void *src, bfdev_size_t n)
{
    const unsigned char *nsrc;
    unsigned char *ndest;

    ndest = dest;
    nsrc = src;

    while (n--)
        *ndest++ = *nsrc++;

    return dest;
}

__bfdev_weak void *
bfport_memmove(void *dest, const void *src, bfdev_size_t n)
{
    char *tmp;
    const char *s;

    if (dest <= src) {
        tmp = dest;
        s = src;

        while (n--)
            *tmp++ = *s++;

        return dest;
    }

    tmp = dest;
    tmp += n;
    s = src;
    s += n;

    while (n--)
        *--tmp = *--s;

    return dest;
}

__bfdev_weak void *
bfport_memset(void *s, int c, bfdev_size_t n)
{
    unsigned char *xs;

    xs = s;
    while (n--)
        *xs++ = c;

    return s;
}

__bfdev_weak int
bfport_memcmp(const void *s1, const void *s2, bfdev_size_t n)
{
    const unsigned char *su1, *su2;
    int res;

    res = 0;
    for (su1 = s1, su2 = s2; 0 < n; ++su1, ++su2, n--) {
        res = *su1 - *su2;
        if (res != 0)
            break;
    }

    return res;
}

__bfdev_weak int
bfport_strcmp(const char *s1, const char *s2)
{
    int cp;

    cp = 0;
    while (!(cp = *s1++ - *s2++)) {
        if (*s1 == '\0')
            break;
    }

    return cp;
}

__bfdev_weak char *
bfport_strchr(const char *s, int c)
{
    for (; *s != (char)c; ++s) {
        if (*s == '\0')
            return BFDEV_NULL;
    }

    return (char *)s;
}

__bfdev_weak bfdev_size_t
bfport_strspn(const char *s, const char *accept)
{
    const unsigned char *p, *a;
    bfdev_size_t count;

    count = 0;
    for (p = s; *p != '\0'; ++p) {
        for (a = accept; *a != '\0'; ++a) {
            if (*p == *a)
                break;
        }
        if (*a == '\0')
            return count;
        ++count;
    }

    return count;
}

__bfdev_weak bfdev_size_t
bfport_strcspn(const char *s, const char *reject)
{
    const unsigned char *p, *r;
    bfdev_size_t count;

    count = 0;
    for (p = s; *p != '\0'; ++p) {
        for (r = reject; *r != '\0'; ++r) {
            if (*p == *r)
                return count;
        }
        ++count;
    }

    return count;
}

__bfdev_weak char *
bfport_strcpy(char *dest, const char *src)
{
    unsigned char *tmp;

    tmp = dest;
    while ((*tmp++ = *src++) != '\0');

    return dest;
}

__bfdev_weak char *
bfport_strncpy(char *dest, const char *src, bfdev_size_t n)
{
    unsigned char *tmp;

    for (tmp = dest; n--; ++tmp) {
        if ((*tmp = *src) != 0)
            src++;
    }

    return dest;
}

__bfdev_weak bfdev_size_t
bfport_strlen(const char *s)
{
    const unsigned char *len;

    len = s;
    while (*len != '\0')
        len++;

    return (char *)len - s;
}

__bfdev_weak bfdev_size_t
bfport_strnlen(const char *s, bfdev_size_t len)
{
    const unsigned char *sc;

    for (sc = s; len--; ++sc) {
        if (*sc == '\0')
            break;
    }

    return (char *)sc - s;
}
