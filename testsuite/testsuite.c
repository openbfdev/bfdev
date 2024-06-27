/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-testsuite"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <bfdev/minmax.h>
#include <bfdev/log.h>
#include <bfdev/errname.h>

#include <testsuite.h>
#include "../examples/time.h"

#define MESSAGE_PASS "\e[0m[\e[32mPASS\e[0m]"
#define MESSAGE_FAIL "\e[0m[\e[31mFAIL\e[0m]"
#define DEFAULT_LOOPS 3

static const struct option
options[] = {
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"count", required_argument, 0, 'c'},
    {"slient", required_argument, 0, 's'},
    { }, /* NULL */
};

static
BFDEV_LIST_HEAD(tests);

#define list_to_testsuite(ptr) \
    bfdev_container_of(ptr, struct testsuite, list)

static struct testsuite *
testsuite_find(const char *name)
{
    struct testsuite *walk;

    bfdev_list_for_each_entry(walk, &tests, list) {
        if (!strcmp(name, walk->name))
            return walk;
    }

    return NULL;
}

static bool
testsuite_exist(struct testsuite *test)
{
    struct testsuite *walk;

    bfdev_list_for_each_entry(walk, &tests, list) {
        if (test == walk)
            return true;
    }

    return false;
}

int
testsuite_register(struct testsuite *test)
{
    if (!test->testing || !test->name)
        return -BFDEV_EINVAL;

    if (testsuite_find(test->name))
        return -BFDEV_EALREADY;

    bfdev_log_info("register '%s'\n", test->name);
    bfdev_list_add_prev(&tests, &test->list);

    return -BFDEV_ENOERR;
}

void
testsuite_unregister(struct testsuite *test)
{
    if (!testsuite_exist(test))
        return;

    bfdev_log_debug("unregister '%s'\n", test->name);
    bfdev_list_del(&test->list);
}

static struct testsuite *
testsuite_iter(const char *name, struct testsuite *iter)
{
    int retval;

    bfdev_list_for_each_entry_continue(iter, &tests, list) {
        retval = strcmp(iter->name, name);
        if (!retval || retval == ':')
            return iter;
    }

    return NULL;
}

static int
trigger_testsuite(struct testsuite *test, unsigned int loops,
                  int argc, const char *argv[])
{
    unsigned int count;
    const char *ename, *einfo;
    void *data;
    int retval;

    bfdev_log_notice("================================\n");
    bfdev_log_notice("Trigger object {%s}\n", test->name);

    data = NULL;
    if (test->prepare) {
        bfdev_log_debug("Preparing...\n");
        EXAMPLE_TIME_STATISTICAL(
            data = test->prepare(argc, argv);
            0;
        );

        retval = BFDEV_PTR_INVAL_ZERO(data);
        bfdev_log_debug("Prepared: (%d) %s\n", retval,
                        retval ? MESSAGE_FAIL : MESSAGE_PASS);
        if (bfdev_unlikely(retval))
            return retval;
    }

    retval = -BFDEV_ENOERR;
    for (count = 1; count <= loops; ++count) {
        bfdev_log_debug("Testing loop%u...\n", count);
        EXAMPLE_TIME_STATISTICAL(
            retval = test->testing(argc, argv, data);
            0;
        );

        bfdev_log_notice("Tested (%d) %s\n", retval,
                         retval ? MESSAGE_FAIL : MESSAGE_PASS);
        if (bfdev_unlikely(retval)) {
            ename = bfdev_errname(retval, &einfo);
            bfdev_log_err("Return status (%s) %s\n", ename, einfo);
            break;
        }
    }

    if (test->release) {
        bfdev_log_notice("Releasing...\n");
        test->release(data);
        bfdev_log_notice("Released.\n");
    }

    return retval;
}

static int
trigger_overall(unsigned int loops)
{
    struct testsuite *walk;
    int retval;

    bfdev_list_for_each_entry(walk, &tests, list) {
        retval = trigger_testsuite(walk, loops, 0, NULL);
        if (bfdev_unlikely(retval))
            return retval;
    }

    return -BFDEV_ENOERR;
}

static __bfdev_noreturn void
usage(void)
{
    struct testsuite *walk;
    unsigned int align;

    bfdev_log_err("Usage: testsuite [option] [group[:name]] ...\n");
    bfdev_log_err("License GPLv2+: GNU GPL version 2 or later.\n");
    bfdev_log_err("\n");

    bfdev_log_err("With no specified, run all testsuits.\n");
    bfdev_log_err("  -v, --version      output version information and exit.\n");
    bfdev_log_err("  -h, --help         display this help and exit.\n");
    bfdev_log_err("\n");

    bfdev_log_err("Mandatory arguments to long options are mandatory for short options too.\n");
    bfdev_log_err("  -c, --count=NUM    number of repetitions for each test\n");
    bfdev_log_err("  -s, --slient       do not print debugging information\n");
    bfdev_log_err("\n");

    align = 0;
    bfdev_list_for_each_entry(walk, &tests, list)
        bfdev_max_adj(align, strlen(walk->name));

    bfdev_log_err("Supported testsuite and description:\n");
    bfdev_list_for_each_entry(walk, &tests, list)
        bfdev_log_err("\t%-*s - %s\n", align + 1, walk->name, walk->desc);

    exit(1);
}

static __bfdev_noreturn void
version(void)
{
    bfdev_log_err("%s\n", bfdev_release());
    exit(1);
}

int
main(int argc, char *const argv[])
{
    unsigned int loops, count;
    int retval, optidx;
    char arg;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    loops = DEFAULT_LOOPS;

    for (;;) {
        arg = getopt_long(argc, argv, "c:svh", options, &optidx);
        if (arg == -1)
            break;

        switch (arg) {
            case 'c':
                loops = atoi(optarg);
                if (!loops) {
                    bfdev_log_err("Invalid loop count: '%s'\n", optarg);
                    usage();
                }
                break;

            case 's':
                bfdev_log_default.record_level = BFDEV_LEVEL_INFO;
                break;

            case 'v':
                version();

            case 'h': default:
                bfdev_log_err("Unknown option: %c\n", arg);
                usage();
        }
    }

    if (optind == argc)
        return trigger_overall(loops);

    for (count = optind; count < argc; ++count) {
        struct testsuite *test;
        char *target;

        target = argv[count];
        test = list_to_testsuite(&tests);

        while ((test = testsuite_iter(target, test))) {
            retval = trigger_testsuite(test, loops, 0, NULL);
            if (retval)
                return retval;
        }
    }

    return -BFDEV_ENOERR;
}
