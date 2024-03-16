/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "matrix-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/matrix.h>
#include "../time.h"

#define TEST_SIZE 40
#define TEST_LOOP 3

#define GENERIC_MATRIX_BENCHMARK(func, name)    \
for (count = 0; count < TEST_LOOP; ++count) {   \
    EXAMPLE_TIME_LOOP(&loop, 1000,              \
        func(&dest, &vara, &varb);              \
    );                                          \
    bfdev_log_info(                             \
        name " %u: %uops/s\n",                  \
        count, loop                             \
    );                                          \
}

int main(int argc, char const *argv[])
{
    BFDEV_MATRIX_TYPE *buffer;
    unsigned int count, loop;

    BFDEV_DEFINE_MATRIX(vara, NULL);
    BFDEV_DEFINE_MATRIX(varb, NULL);
    BFDEV_DEFINE_MATRIX(dest, NULL);

    buffer = malloc(TEST_SIZE * TEST_SIZE * BFDEV_MATRIX_SIZE);
    if (!buffer)
        return 1;

    srand(time(NULL));
    for (count = 0; count < TEST_SIZE * TEST_SIZE; ++count)
        buffer[count] = (uint16_t)rand();
    bfdev_matrix_import(&vara, buffer, TEST_SIZE, TEST_SIZE);

    for (count = 0; count < TEST_SIZE * TEST_SIZE; ++count)
        buffer[count] = (uint16_t)rand();
    bfdev_matrix_import(&varb, buffer, TEST_SIZE, TEST_SIZE);

    GENERIC_MATRIX_BENCHMARK(bfdev_matrix_add, "adding")
    GENERIC_MATRIX_BENCHMARK(bfdev_matrix_sub, "subtracting")
    GENERIC_MATRIX_BENCHMARK(bfdev_matrix_mul, "multiplying")

    bfdev_matrix_release(&vara);
    bfdev_matrix_release(&varb);
    bfdev_matrix_release(&dest);
    free(buffer);

    return 0;
}
