/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <bfdev/bloom.h>
#include <bfdev/stringhash.h>

#define TEST_LOOP 50
#define TEST_SIZE 100

static unsigned int
bloom_hash(const void *key, void *pdata)
{
    return bfdev_pjwhash(key);
}

int main(void)
{
    struct bfdev_bloom *bloom;
    char buffer[TEST_SIZE][32];
    unsigned int count;
    bool retval;

    bloom = bfdev_bloom_create(NULL, TEST_SIZE, bloom_hash, NULL);
    if (!bloom)
        err(errno, "bfdev_bloom_create");

    for (count = 0; count < TEST_LOOP; ++count) {
        snprintf(buffer[count], sizeof(*buffer), "%d", rand());
        retval = bfdev_bloom_push(bloom, buffer[count]);
        printf("push %u: %s (%s)\n", count, buffer[count],
                retval ? "clash" : "none");
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_bloom_peek(bloom, buffer[count]);
        printf("peek %u: %s (%s)\n", count, buffer[count],
                retval ? "passed" : "failed");
        if (!retval)
            return 1;
    }

    bfdev_bloom_destory(bloom);
    return 0;
}
