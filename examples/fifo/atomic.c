/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <bfdev/fifo.h>

#define TEST_SIZE 16
#define TEST_LOOP (TEST_SIZE * 64)

static
BFDEV_DEFINE_FIFO(fifo_test, char, TEST_SIZE);

static void *
fifo_production(void *unused)
{
    unsigned int count, index;
    char ch;

    for (count = index = 0; count < TEST_LOOP; ++count) {
        while (bfdev_fifo_check_full(&fifo_test))
            sched_yield();

        index = count % TEST_SIZE;
        ch = "123456789ABCDEF\n"[index];
        bfdev_fifo_put(&fifo_test, ch);
    }

    while (bfdev_fifo_check_full(&fifo_test))
        sched_yield();
    bfdev_fifo_put(&fifo_test, 0);

    return NULL;
}

int main(int argc, const char *argv[])
{
    pthread_t thread;
    char ch;

    pthread_create(&thread, NULL, fifo_production, NULL);
    for (;;) {
        while (bfdev_fifo_check_empty(&fifo_test))
            sched_yield();

        bfdev_fifo_get(&fifo_test, &ch);
        if (!ch) {
            printf("exit\n");
            break;
        }

        printf("%c", ch);
    }

    return 0;
}
