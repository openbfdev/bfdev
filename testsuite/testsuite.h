/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TESTSUITE_H_
#define _BFDEV_TESTSUITE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>
#include <bfdev/errptr.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

typedef int
(testsuite_testing)(int argc, const char *argv[], void *data);

typedef void *
(testsuite_prepare)(int argc, const char *argv[]);

typedef void
(testsuite_release)(void *data);

struct testsuite {
    bfdev_list_head_t list;
    const char *name;
    const char *desc;

    testsuite_testing *testing;
    testsuite_prepare *prepare;
    testsuite_release *release;
};

extern int
testsuite_register(struct testsuite *test);

extern void
testsuite_unregister(struct testsuite *test);

#define TESTSUITE(NAME, PREPARE, RELEASE, DESC) \
static testsuite_testing                        \
__BFDEV_UNIQUE_ID(testing);                     \
                                                \
static struct testsuite                         \
__BFDEV_UNIQUE_ID(testsuite) = {                \
    .name = (NAME), .desc = (DESC),             \
    .testing = __BFDEV_UNIQUE_ID(testing),      \
    .prepare = (PREPARE),                       \
    .release = (RELEASE),                       \
};                                              \
                                                \
static __bfdev_ctor int                         \
__BFDEV_UNIQUE_ID(initcall)(void)               \
{                                               \
    return testsuite_register(                  \
        &__BFDEV_UNIQUE_ID(testsuite)           \
    );                                          \
}                                               \
                                                \
static __bfdev_dtor void                        \
__BFDEV_UNIQUE_ID(exitcall)(void)               \
{                                               \
    testsuite_unregister(                       \
        &__BFDEV_UNIQUE_ID(testsuite)           \
    );                                          \
}                                               \
                                                \
static int __BFDEV_UNIQUE_ID(testing)           \
(int argc, const char *argv[], void *data)

BFDEV_END_DECLS

#endif /* _BFDEV_TESTSUITE_H_ */
