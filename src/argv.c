/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/argv.h>
#include <export.h>

#define ARGV_SEPARA " \t\r\n"

export unsigned int
bfdev_argv_count(const char *args)
{
    unsigned int argc;
    size_t offset;

    argc = 0;
    for (;;) {
        offset = bfport_strspn(args, ARGV_SEPARA);
        if (!args[offset])
            break;
        args += offset;
        argc++;

        offset = bfport_strcspn(args, ARGV_SEPARA);
        if (!args[offset])
            break;
        args += offset;
    }

    return argc;
}

export char **
bfdev_argv_split(const bfdev_alloc_t *alloc, const char *args,
                 unsigned int *argcp)
{
    unsigned int count, argc;
    char *block, **argv;

    argc = bfdev_argv_count(args);
    count = (argc + 1) * sizeof(*argv);

    argv = bfdev_malloc(alloc, count + bfport_strlen(args) + 1);
    if (bfdev_unlikely(!argv))
        return NULL;

    block = (void *)argv + count;
    bfport_strcpy(block, args);

    for (count = 0; count < argc; ++count) {
        block += bfport_strspn(block, ARGV_SEPARA);
        argv[count] = block;
        block += bfport_strcspn(block, ARGV_SEPARA);
        *block++ = '\0';
    }

    argv[count] = NULL;

    if (argcp)
        *argcp = argc;

    return argv;
}

export void
bfdev_argv_destory(const bfdev_alloc_t *alloc, char **argv)
{
    bfdev_free(alloc, argv);
}
