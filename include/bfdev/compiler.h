/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_COMPILER_H_
#define _BFDEV_COMPILER_H_

#include <bfdev/config.h>
#include <bfdev/compiler/types.h>

BFDEV_BEGIN_DECLS

struct bfdev_branch {
    const char *func;
    const char *file;
    unsigned int line;
};

#ifndef bfdev_likely_notrace
# define bfdev_likely_notrace(x) __builtin_expect(!!(x), 1)
# define bfdev_unlikely_notrace(x) __builtin_expect(!!(x), 0)
#endif

#ifndef BFDEV_DEBUG_BRANCH
# define bfdev_likely(x) bfdev_likely_notrace(x)
# define bfdev_unlikely(x) bfdev_unlikely_notrace(x)
#else /* !BFDEV_DEBUG_BRANCH */

extern void
bfdev_likely_update(const struct bfdev_branch *data, int condition,
                    int expect, int constant);

#define __bfdev_branch_check__(x, expect, constant) ({  \
    bool __condition;                                   \
    static struct bfdev_branch __branch_data = {        \
        .func = __func__,                               \
        .file = __FILE__,                               \
        .line = __LINE__,                               \
    };                                                  \
                                                        \
	__condition = __builtin_expect(!!(x), expect);	    \
    bfdev_likely_update(&__branch_data, __condition,    \
                        expect, constant);              \
    __condition;                                        \
})

#define bfdev_likely(x) (__bfdev_branch_check__(x, 1, __builtin_constant_p(x)))
#define bfdev_unlikely(x) (__bfdev_branch_check__(x, 0, __builtin_constant_p(x)))

#endif

/*
 * Optimization barrier
 * The "volatile" is due to gcc bugs
 */
#ifndef bfdev_barrier
# define bfdev_barrier() __asm__ __volatile__("": : :"memory")
#endif

/* Not-quite-unique ID. */
#ifndef __BFDEV_UNIQUE_ID
# define __BFDEV_UNIQUE_ID(prefix) __BFDEV_PASTE(__BFDEV_PASTE(__UNIQUE_ID_, prefix), __LINE__)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_COMPILER_H_ */
