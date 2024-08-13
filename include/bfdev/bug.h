/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BUG_H_
#define _BFDEV_BUG_H_

#include <bfdev/config.h>
#include <bfdev/stdlib.h>
#include <bfdev/log.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_BUG_MSG
# define BFDEV_BUG_MSG(msg...) do {     \
    bfdev_log_alert(msg);               \
    abort();                            \
} while (0)
#endif

#ifndef BFDEV_WARN_MSG
# define BFDEV_WARN_MSG(msg...) do {    \
    bfdev_log_alert(msg);               \
} while (0)
#endif

#ifndef BFDEV_BUG
# define BFDEV_BUG()                                    \
BFDEV_BUG_MSG(                                          \
    "Generic BUG: failure at %s:%d, in func '%s'\n",    \
    __FILE__, __LINE__, __FUNCTION__                    \
)
#endif

#ifndef BFDEV_WARN
# define BFDEV_WARN()                                   \
BFDEV_WARN_MSG(                                         \
    "Generic WARN: failure at %s:%d, in func '%s'\n",   \
    __FILE__, __LINE__, __FUNCTION__                    \
)
#endif

#ifndef BFDEV_BUG_ON
# define BFDEV_BUG_ON(condition) ({     \
    bool __cond = !!(condition);        \
    if (bfdev_unlikely(__cond))         \
        BFDEV_BUG();                    \
    bfdev_unlikely(__cond);             \
})
#endif

#ifndef BFDEV_WARN_ON
# define BFDEV_WARN_ON(condition) ({    \
    bool __cond = !!(condition);        \
    if (bfdev_unlikely(__cond))         \
        BFDEV_WARN();                   \
    bfdev_unlikely(__cond);             \
})
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_BUG_H_ */
