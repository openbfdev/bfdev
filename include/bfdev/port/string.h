/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STRING_H_
#define _BFDEV_PORT_STRING_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <string.h>
#endif

BFDEV_BEGIN_DECLS

#ifndef bfport_memcpy
# define bfport_memcpy bfport_memcpy
static __bfdev_always_inline void *
bfport_memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    return memcpy(dest, src, n);
}
#endif

#ifndef bfport_memset
# define bfport_memset bfport_memset
static __bfdev_always_inline void *
bfport_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}
#endif

#ifndef bfport_memcmp
# define bfport_memcmp bfport_memcmp
static __bfdev_always_inline int
bfport_memcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}
#endif

#ifndef bfport_strcmp
# define bfport_strcmp bfport_strcmp
static __bfdev_always_inline int
bfport_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}
#endif

#ifndef bfport_strchr
# define bfport_strchr bfport_strchr
static __bfdev_always_inline char *
bfport_strchr(const char *s, int c)
{
    return strchr(s, c);
}
#endif

#ifndef bfport_strspn
# define bfport_strspn bfport_strspn
static __bfdev_always_inline size_t
bfport_strspn(const char *s, const char *accept)
{
    return strspn(s, accept);
}
#endif

#ifndef bfport_strcspn
# define bfport_strcspn bfport_strcspn
static __bfdev_always_inline size_t
bfport_strcspn(const char *s, const char *reject)
{
    return strcspn(s, reject);
}
#endif

#ifndef bfport_strcpy
# define bfport_strcpy bfport_strcpy
static __bfdev_always_inline char *
bfport_strcpy(char *restrict dest, const char *restrict src)
{
    return strcpy(dest, src);
}
#endif

#ifndef bfport_strncpy
# define bfport_strncpy bfport_strncpy
static __bfdev_always_inline char *
bfport_strncpy(char *restrict dest, const char *restrict src, size_t n)
{
    return strncpy(dest, src, n);
}
#endif

#ifndef bfport_strlen
# define bfport_strlen bfport_strlen
static __bfdev_always_inline size_t
bfport_strlen(const char *s)
{
    return strlen(s);
}
#endif

#ifndef bfport_strnlen
# define bfport_strnlen bfport_strnlen
static __bfdev_always_inline size_t
bfport_strnlen(const char *s, size_t len)
{
    return strnlen(s, len);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STRING_H_ */
