/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDLIB_H_
#define _BFDEV_PORT_STDLIB_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <stdlib.h>
#endif

BFDEV_BEGIN_DECLS

#ifndef bfport_rand
# define bfport_rand bfport_rand
static __bfdev_always_inline int
bfport_rand()
{
    return rand();
}
#endif

#ifndef bfport_malloc
# define bfport_malloc bfport_malloc
static __bfdev_always_inline __bfdev_malloc void *
bfport_malloc(size_t size)
{
    return malloc(size);
}
#endif

#ifndef bfport_calloc
# define bfport_calloc bfport_calloc
static __bfdev_always_inline __bfdev_malloc void *
bfport_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}
#endif

#ifndef bfport_realloc
# define bfport_realloc bfport_realloc
static __bfdev_always_inline __bfdev_malloc void *
bfport_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}
#endif

#ifndef bfport_free
# define bfport_free bfport_free
static __bfdev_always_inline void
bfport_free(void *ptr)
{
    free(ptr);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDLIB_H_ */
