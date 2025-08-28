/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "atomic-spinlock"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/atomic.h>
#include <bfdev/cmpxchg.h>
#include <bfdev/log.h>
#include <pthread.h>
#include <unistd.h>

static
bfdev_atomic_t lock;

static volatile
long counter;

static void
spin_lock(bfdev_atomic_t *lock)
{
    while (bfdev_cmpxchg(lock, 1, 0) != 1)
        sched_yield();
}

static void
spin_unlock(bfdev_atomic_t *lock)
{
    if (bfdev_cmpxchg(lock, 0, 1) == 0)
        sched_yield();
}

static void *
thread1_task(void *unused)
{
    unsigned int time;

    for (time = 0; time < 100000; ++time) {
        spin_lock(&lock);
        counter++;
        spin_unlock(&lock);
    }

    return NULL;
}

static void *
thread2_task(void *unused)
{
    unsigned int time;

    for (time = 0; time < 100000; ++time) {
        spin_lock(&lock);
        counter--;
        spin_unlock(&lock);
    }

    return NULL;
}

int
main(int argc, const char *argv[])
{
    pthread_t thread1, thread2;

    bfdev_atomic_write(&lock, 1);
    pthread_create(&thread1, NULL, thread1_task, NULL);
    pthread_create(&thread2, NULL, thread2_task, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    bfdev_log_info("counter: %ld\n", counter);
    if (counter)
        return 1;

    return 0;
}
