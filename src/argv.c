/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/argv.h>
#include <export.h>

#define ARGV_SEPARA " \t\r\n"

export unsigned int
bfdev_argv_count(const char *args)
{
    unsigned int argc = 0;
    size_t offset;

    for (;;) {
        offset = strspn(args, ARGV_SEPARA);
        if (!args[offset])
            break;
        args += offset;
        argc++;

        offset = strcspn(args, ARGV_SEPARA);
        if (!args[offset])
            break;
        args += offset;
    }

    return argc;
}

export char **
bfdev_argv_split(const struct bfdev_alloc *alloc, const char *args,
                 unsigned int *argcp)
{
    unsigned int count, argc;
    char *block, **argv;

    argc = bfdev_argv_count(args);
    count = (argc + 1) * sizeof(*argv);

    argv = bfdev_malloc(alloc, count + strlen(args) + 1);
    if (bfdev_unlikely(!argv))
        return NULL;

    block = (void *)argv + count;
    strcpy(block, args);

    for (count = 0; count < argc; ++count) {
        block += strspn(block, ARGV_SEPARA);
        argv[count] = block;
        block += strcspn(block, ARGV_SEPARA);
        *block++ = '\0';
    }

    argv[count] = NULL;

    if (argcp)
        *argcp = argc;

    return argv;
}
