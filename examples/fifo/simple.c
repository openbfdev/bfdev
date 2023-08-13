/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <bfdev/fifo.h>

static const char test_table[] = {
    'b', 'f', 'd', 'e', 'v', '-', 'f', 'i',
    'f', 'o', '-', 't', 'e', 's', 't', '\n',
};

#define TEST_LOOP 64
BFDEV_DEFINE_FIFO(normal_bytetest, char, BFDEV_ARRAY_SIZE(test_table));

static void *fifo_production(void *unused)
{
    unsigned int count, index;
    char ch;

    for (count = 0; count < TEST_LOOP; ++count) {
        for (index = 0; index < BFDEV_ARRAY_SIZE(test_table); ++index) {
            while (bfdev_fifo_check_full(&normal_bytetest))
                sched_yield();
            ch = test_table[index];
            bfdev_fifo_put(&normal_bytetest, ch);
        }
    }

    while (bfdev_fifo_check_full(&normal_bytetest))
        sched_yield();
    bfdev_fifo_put(&normal_bytetest, 0);

    return NULL;
}

int main(void)
{
    pthread_t thread;
    char ch;

    pthread_create(&thread, NULL, fifo_production, NULL);
    for (;;) {
        while (bfdev_fifo_check_empty(&normal_bytetest))
            sched_yield();
        bfdev_fifo_get(&normal_bytetest, &ch);
        if (!ch) {
            printf("exit\n");
            break;
        }
        printf("%c", ch);
    }

    return 0;
}
