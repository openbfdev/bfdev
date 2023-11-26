/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/string.h>
#include <export.h>

export char *
bfdev_strdup(const struct bfdev_alloc *alloc,
             const char *string)
{
    size_t length;
    char *dump;

    if (bfdev_unlikely(!string))
        return NULL;

    length = strlen(string);
    dump = bfdev_malloc(alloc, length + 1);

    if (bfdev_likely(dump))
        strcpy(dump, string);

    return dump;
}

export char *
bfdev_strndup(const struct bfdev_alloc *alloc,
              const char *string, size_t len)
{
    size_t length;
    char *dump;

    if (bfdev_unlikely(!string))
        return NULL;

    length = strnlen(string, len);
    dump = bfdev_malloc(alloc, length + 1);

    if (bfdev_likely(dump))
        strncpy(dump, string, len);

    return dump;
}
