/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/string.h>
#include <export.h>

export char *
bfdev_strdup(const bfdev_alloc_t *alloc,
             const char *string)
{
    size_t length;
    char *dump;

    if (bfdev_unlikely(!string))
        return NULL;

    length = bfport_strlen(string);
    dump = bfdev_malloc(alloc, length + 1);

    if (bfdev_likely(dump))
        bfport_strcpy(dump, string);

    return dump;
}

export char *
bfdev_strndup(const bfdev_alloc_t *alloc,
              const char *string, size_t len)
{
    size_t length;
    char *dump;

    if (bfdev_unlikely(!string))
        return NULL;

    length = bfport_strnlen(string, len);
    dump = bfdev_malloc(alloc, length + 1);

    if (bfdev_likely(dump))
        bfport_strncpy(dump, string, len);

    return dump;
}
