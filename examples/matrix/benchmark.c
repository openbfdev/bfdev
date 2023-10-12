/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "crc-bandwidth"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <bfdev/log.h>
#include <bfdev/matrix.h>

#define TEST_SIZE 40
#define TEST_LOOP 3

#define GENERIC_MATRIX_BENCHMARK(func, name)                \
for (count = 0; count < TEST_LOOP; ++count) {               \
    gettimeofday(&etval, NULL);                             \
    etval.tv_sec++;                                         \
    loop = 0;                                               \
                                                            \
    do {                                                    \
        result = func(NULL, vara, varb);                    \
        bfdev_matrix_destory(NULL, result);                 \
        loop++;                                             \
        gettimeofday(&stval, NULL);                         \
    } while (timercmp(&stval, &etval, <));                  \
    bfdev_log_info(name " %u: %uops/s\n", count, loop);     \
}

int main(int argc, char const *argv[])
{
    struct timeval stval, etval;
    unsigned int count, loop;
    struct bfdev_matrix *vara, *varb;
    struct bfdev_matrix *result;

    vara = bfdev_matrix_create(NULL, TEST_SIZE, TEST_SIZE);
    if (!vara)
        return 1;

    varb = bfdev_matrix_create(NULL, TEST_SIZE, TEST_SIZE);
    if (!varb)
        return 1;

    srand(time(NULL));
    for (count = 0; count < TEST_SIZE * TEST_SIZE; ++count) {
        vara->values[count] = (uint64_t)rand() << 32 | rand();
        varb->values[count] = (uint64_t)rand() << 32 | rand();
    }

    GENERIC_MATRIX_BENCHMARK(bfdev_matrix_add, "adding")
    GENERIC_MATRIX_BENCHMARK(bfdev_matrix_sub, "subtracting")
    GENERIC_MATRIX_BENCHMARK(bfdev_matrix_mul, "multiplying")

    bfdev_matrix_destory(NULL, vara);
    bfdev_matrix_destory(NULL, varb);

    return 0;
}
