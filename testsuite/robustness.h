/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _EXAMPLES_ROBUSTNESS_H_
#define _EXAMPLES_ROBUSTNESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/allocator.h>
#include <bfdev/log.h>

struct {
    unsigned long alloced;
    unsigned long depletion;
    unsigned long curr;
} rob;

static void *
robustness_malloc(size_t size, void *pdata)
{
    void *result;

    if (rob.curr > rob.depletion) {
        bfdev_log_emerg("malloc depletion ignored\n");
        exit(1);
    }

    if (rob.curr++ == rob.depletion) {
        bfdev_log_debug("malloc depletion trigger\n");
        return NULL;
    }

    result = malloc(size);
    if (!result)
        return NULL;

    rob.alloced++;
    return result;
}

static void *
robustness_zalloc(size_t size, void *pdata)
{
    void *result;

    if (rob.curr > rob.depletion) {
        bfdev_log_emerg("zalloc depletion ignored\n");
        exit(1);
    }

    if (rob.curr++ == rob.depletion) {
        bfdev_log_debug("zalloc depletion trigger\n");
        return NULL;
    }

    result = calloc(1, size);
    if (!result)
        return NULL;

    rob.alloced++;
    return result;
}

static void *
robustness_realloc(void *block, size_t resize, void *pdata)
{
    void *result;

    if (rob.curr > rob.depletion) {
        bfdev_log_emerg("realloc depletion ignored\n");
        exit(1);
    }

    if (rob.curr++ == rob.depletion) {
        bfdev_log_debug("realloc depletion trigger\n");
        return NULL;
    }

    result = realloc(block, resize);
    if (!result)
        return NULL;

    return result;

}

static void
robustness_free(void *block, void *pdata)
{
    if (rob.alloced--)
        return;

    bfdev_log_emerg("free leaking\n");
    exit(1);
}

extern int
robustness_main(int argc, const char *argv[]);

int
main(int argc, const char *argv[])
{
    bool depletion;
    int retval;

    bfdev_alloc_default_ops = (bfdev_alloc_ops_t) {
        .alloc = robustness_malloc,
        .zalloc = robustness_zalloc,
        .realloc = robustness_realloc,
        .free = robustness_free,
    };

    for (;;) {
        retval = robustness_main(argc, argv);
        depletion = rob.curr > rob.depletion;

        if (retval) {
            if (!depletion)
                return retval;

            rob.depletion++;
            rob.curr = 0;
            continue;
        }

        if (rob.alloced) {
            bfdev_log_emerg("allocator leakage\n");
            return 1;
        }

        if (depletion) {
            bfdev_log_emerg("allocator depletion without error\n");
            return 1;
        }

        return 0;
    }

    return 0;
}

#endif /* _EXAMPLES_ROBUSTNESS_H_ */
