/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-sha1"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/log.h>
#include <bfdev/errname.h>
#include <bfdev/sha1.h>

int
main(int argc, const char *argv[])
{
    unsigned int index;

    for (index = 1; index < argc; ++index) {
        const char *para;

        para = argv[index];
        if (para[0] != '-')
            break;

        switch (para[1]) {
            case 'h': default:
                goto usage;
        }
    }

    if (index == argc)
        goto usage;

    do {
        bfdev_sha1_ctx_t sha1;
        uint8_t digest[BFDEV_SHA1_DIGEST_SIZE];
        const char *data;
        unsigned int count;
        size_t length;

        data = argv[index];
        length = strlen(data);

        bfdev_sha1_init(&sha1);
        bfdev_sha1_update(&sha1, data, length);
        bfdev_sha1_finish(&sha1, digest);

        for (count = 0; count < BFDEV_SHA1_DIGEST_SIZE; ++count)
            printf("%02x", digest[count]);
        fputc('\n', stdout);
    } while (++index < argc);

    return 0;

usage:
    bfdev_log_err("Usage: %s [-h] data ...\n", argv[0]);
    return 1;
}
