/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "prandom-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <time.h>
#include <bfdev/log.h>
#include <bfdev/prandom.h>

#define TEST_LOOP 100

BFDEV_DEFINE_PRANDOM(prandom);

int main(int argc, const char *argv[])
{
    unsigned int count;
    uint64_t seed;
    uint32_t value;

    seed = ((uint64_t)time(NULL) << 32) ^ time(NULL);
    bfdev_log_info("prandom seed: %#018llx\n", (long long)seed);
    bfdev_prandom_seed(&prandom, seed);

    for (count = 0; count < TEST_LOOP; ++count) {
        value = bfdev_prandom_value(&prandom);
        bfdev_log_info("prandom value: %#010x\n", value);
    }

    return 0;
}
