/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <bfdev/ringbuf.h>

static const char test_table[32] = {
    'b', 'f', 'd', 'e', 'v', '-', 'r', 'i',
    'n', 'g', '-', 't', 'e', 's', 't', '\n',
    'b', 'f', 'd', 'e', 'v', '-', 'r', 'i',
    'n', 'g', '-', 't', 'e', 's', 't', '\n',
};

static const char warn_table[16] = {
    '=', '=', '=', '=', '=', '=', '=', '=',
    '=', '=', '=', '=', '=', '=', '=', '\n',
};

#define TEST_SIZE 16
#define TEST_LOOP 64
BFDEV_DEFINE_RINGBUF(normal_bytetest, char, TEST_SIZE);

static void
ringbuf_warnup(void)
{
    unsigned int count;

    bfdev_ringbuf_homing(&normal_bytetest);
    for (count = 0; count < TEST_SIZE; ++count)
        bfdev_ringbuf_put(&normal_bytetest, warn_table[count]);
}

int
main(int argc, const char *argv[])
{
    unsigned int count, index;
    char ch = 0;

    ringbuf_warnup();
    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_ringbuf_peek(&normal_bytetest, &ch);
        if (!ch)
            return 1;
        printf("%c", ch);
        index = count % TEST_SIZE;
        bfdev_ringbuf_put(&normal_bytetest, test_table[index]);
    }

    return 0;
}
