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

static BFDEV_LIST_HEAD(tests);

#define list_to_testsuite(ptr) \
    bfdev_container_of(ptr, struct testsuite, list)

static long
testsuite_cmp(const bfdev_list_head_t *node1,
              const bfdev_list_head_t *node2, void *pdata)
{
    struct testsuite *test1, *test2;
    int retval;

    test1 = list_to_testsuite(node1);
    test2 = list_to_testsuite(node2);

    retval = strcmp(test1->name, test2->name);
    if (retval)
        return retval;

    return 0;
}

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
    bfdev_list_add(&tests, &test->list);

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

    return -BFDEV_ENOERR;
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

static void
usage(void)
{
    struct testsuite *walk;
    unsigned int align;

    bfdev_log_print("usage: testsuite [option] [group[:name]] ...\n");
    bfdev_log_print("\t-c  <count>  number of repetitions for each test\n");
    bfdev_log_print("\t-d  do not print debugging information\n");
    bfdev_log_print("\t-h  show this help\n\n");

    align = 0;
    bfdev_list_for_each_entry(walk, &tests, list)
        bfdev_max_adj(align, strlen(walk->name));

    bfdev_log_print("Supported tests:\n");
    bfdev_list_for_each_entry(walk, &tests, list)
        bfdev_log_print("\t%-*s - %s\n", align + 1, walk->name, walk->desc);
}

int
main(int argc, char *const argv[])
{
    unsigned int loops, count;
    char arg;
    int retval;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    loops = DEFAULT_LOOPS;
    bfdev_list_sort(&tests, testsuite_cmp, NULL);

    for (;;) {
        arg = getopt(argc, argv, "c:dh");
        if (arg == -1)
            break;

        switch (arg) {
            case 'c':
                loops = atoi(optarg);
                if (!loops)
                    goto usage;
                break;

            case 'd':
                bfdev_log_default.record_level = BFDEV_LEVEL_INFO;
                break;

            case 'h': default:
                goto usage;
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

usage:
    usage();
    return -BFDEV_EINVAL;
}
