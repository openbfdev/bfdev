/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PORT_STDIO_H_
#define _BFDEV_PORT_STDIO_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <stdio.h>
#endif

BFDEV_BEGIN_DECLS

#ifndef bfport_file
# define bfport_file FILE
#endif

#ifndef bfport_stdout
# define bfport_stdout stdout
#endif

#ifndef bfport_stderr
# define bfport_stderr stderr
#endif

#ifndef bfport_fwrite
# define bfport_fwrite bfport_fwrite
static __bfdev_always_inline unsigned long
bfport_fwrite(const void *restrict ptr, size_t size,
              size_t n, FILE *restrict stream)
{
    return fwrite(ptr, size, n, stream);
}
#endif

#ifndef bfport_fflush
# define bfport_fflush bfport_fflush
static __bfdev_always_inline int
bfport_fflush(FILE *restrict stream)
{
    return fflush(stream);
}
#endif

#ifndef bfport_vsnprintf
# define bfport_vsnprintf bfport_vsnprintf
static __bfdev_always_inline int
bfport_vsnprintf(char *restrict s, size_t maxlen,
                 const char *restrict format, va_list arg)
{
    return vsnprintf(s, maxlen, format, arg);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_PORT_STDIO_H_ */
