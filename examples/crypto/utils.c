/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-crypto"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <string.h>
#include <bfdev/log.h>
#include <bfdev/errname.h>
#include <bfdev/sha1.h>
#include <bfdev/sha2.h>
#include <bfdev/md5.h>

enum {
    ALGO_SHA1 = 0,
    ALGO_SHA224,
    ALGO_SHA256,
    ALGO_MD5,
};

int
main(int argc, const char *argv[])
{
    unsigned int index, algo;

    algo = ALGO_SHA1;
    for (index = 1; index < argc; ++index) {
        const char *para;

        para = argv[index];
        if (para[0] != '-')
            break;

        switch (para[1]) {
            case 'l':
                algo = ALGO_SHA224;
                break;

            case 'e':
                algo = ALGO_SHA256;
                break;

            case 'm':
                algo = ALGO_MD5;
                break;

            case 'h': default:
                goto usage;
        }
    }

    if (index == argc)
        goto usage;

    do {
        const char *data;
        size_t length;

        data = argv[index];
        length = strlen(data);

        switch (algo) {
            case ALGO_SHA1: default: {
                bfdev_sha1_ctx_t sha1;
                uint8_t digest[BFDEV_SHA1_DIGEST_SIZE];
                unsigned int count;

                bfdev_sha1_init(&sha1);
                bfdev_sha1_update(&sha1, data, length);
                bfdev_sha1_finish(&sha1, digest);

                for (count = 0; count < BFDEV_SHA1_DIGEST_SIZE; ++count)
                    printf("%02x", digest[count]);
                fputc('\n', stdout);
                break;
            }

            case ALGO_SHA224: {
                bfdev_sha2_ctx_t sha2;
                uint8_t digest[BFDEV_SHA224_DIGEST_SIZE];
                unsigned int count;

                bfdev_sha224_init(&sha2);
                bfdev_sha2_update(&sha2, data, length);
                bfdev_sha224_finish(&sha2, digest);

                for (count = 0; count < BFDEV_SHA224_DIGEST_SIZE; ++count)
                    printf("%02x", digest[count]);
                fputc('\n', stdout);
                break;
            }

            case ALGO_SHA256: {
                bfdev_sha2_ctx_t sha2;
                uint8_t digest[BFDEV_SHA256_DIGEST_SIZE];
                unsigned int count;

                bfdev_sha256_init(&sha2);
                bfdev_sha2_update(&sha2, data, length);
                bfdev_sha256_finish(&sha2, digest);

                for (count = 0; count < BFDEV_SHA256_DIGEST_SIZE; ++count)
                    printf("%02x", digest[count]);
                fputc('\n', stdout);
                break;
            }

            case ALGO_MD5: {
                bfdev_md5_ctx_t md5;
                uint8_t digest[BFDEV_MD5_DIGEST_SIZE];
                unsigned int count;

                bfdev_md5_init(&md5);
                bfdev_md5_update(&md5, data, length);
                bfdev_md5_finish(&md5, digest);

                for (count = 0; count < BFDEV_MD5_DIGEST_SIZE; ++count)
                    printf("%02x", digest[count]);
                fputc('\n', stdout);
                break;
            }
        }
    } while (++index < argc);

    return 0;

usage:
    bfdev_log_err("Usage: %s [-h -l -e] data ...\n", argv[0]);
    bfdev_log_err("\t-l  using SHA224 algorithm\n");
    bfdev_log_err("\t-e  using SHA256 algorithm\n");
    bfdev_log_err("\t-u  using MD5 algorithm\n");
    bfdev_log_err("The default algorithm is SHA1\n");
    return 1;
}
