/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bfdev/macro.h>
#include <bfdev/atomic.h>
#include <bfdev/circle.h>

#define TEST_SIZE 16
#define TEST_LOOP (TEST_SIZE * 64)

static char buffer[TEST_SIZE];
static BFDEV_DEFINE_CIRCLE(test_circle, buffer);

static void *
fifo_production(void *unused)
{
    unsigned int index, count;

    for (count = 0; count < TEST_LOOP; ++count) {
        while (!BFDEV_CIRCLE_SPACE(
                BFDEV_READ_ONCE(test_circle.head),
                BFDEV_READ_ONCE(test_circle.tail),
                TEST_SIZE
            )
        ) sched_yield();

        index = BFDEV_READ_ONCE(test_circle.head) % TEST_SIZE;
        ((char *)test_circle.buffer)[index] = "123456789ABCDEF\n"[index];
        BFDEV_WRITE_ONCE(test_circle.head, test_circle.head + 1);
    }

    while (!BFDEV_CIRCLE_SPACE(
            BFDEV_READ_ONCE(test_circle.head),
            BFDEV_READ_ONCE(test_circle.tail),
            TEST_SIZE
        )
    ) sched_yield();

    index = BFDEV_READ_ONCE(test_circle.head) % TEST_SIZE;
    ((char *)test_circle.buffer)[index] = 0;
    BFDEV_WRITE_ONCE(test_circle.head, test_circle.head + 1);

    return NULL;
}

int main(int argc, const char *argv[])
{
    unsigned int index;
    pthread_t thread;
    char ch;

    pthread_create(&thread, NULL, fifo_production, NULL);
    for (;;) {
        while (!BFDEV_CIRCLE_CNT(
                BFDEV_READ_ONCE(test_circle.head),
                BFDEV_READ_ONCE(test_circle.tail),
                TEST_SIZE
            )
        ) sched_yield();

        index = BFDEV_READ_ONCE(test_circle.tail) % TEST_SIZE;
        ch = ((char *)test_circle.buffer)[index];
        BFDEV_WRITE_ONCE(test_circle.tail, test_circle.tail + 1);

        if (!ch) {
            printf("exit\n");
            break;
        }

        printf("%c", ch);
    }

    return 0;
}
