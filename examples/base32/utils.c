/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-base32"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/log.h>
#include <bfdev/errname.h>
#include <bfdev/base32.h>

int
main(int argc, const char *argv[])
{
    unsigned int index;
    bool decode;
    int retval;

    decode = false;
    for (index = 1; index < argc; ++index) {
        const char *para;

        para = argv[index];
        if (para[0] != '-')
            break;

        switch (para[1]) {
            case 'd':
                decode = true;
                break;

            case 'h': default:
                goto usage;
        }
    }

    if (index == argc)
        goto usage;

    do {
        size_t len1, len2;
        const char *data;
        char *buff;

        data = argv[index];
        len1 = strlen(data);

        if (decode)
            len2 = bfdev_base32_decode_length(len1);
        else
            len2 = bfdev_base32_encode_length(len1);

        buff = malloc(len2);
        if (!buff) {
            bfdev_log_err("Out of memory.\n");
            return 1;
        }

        if (!decode)
            bfdev_base32_encode(buff, data, len1);
        else {
            retval = bfdev_base32_decode(buff, data, len1);
            if (retval) {
                const char *ername, *infop;

                ername = bfdev_errname(retval, &infop);
                bfdev_log_err("Decode error: %s (%s).\n", ername, infop);

                return retval;
            }
        }

        fwrite(buff, 1, len2, stdout);
        fputc('\n', stdout);
        free(buff);
    } while (++index < argc);

    return 0;

usage:
    bfdev_log_err("Usage: %s [-d] data ...\n", argv[0]);
    return 1;
}
