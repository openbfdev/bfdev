/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _EXAMPLES_TIME_H_
#define _EXAMPLES_TIME_H_

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <bfdev/log.h>

static inline void
time_dump(double ticks, struct tms *stms, struct tms *etms,
          struct timeval *start, struct timeval *stop)
{
    uint64_t tstart, tstop;

    tstart = (uint64_t)start->tv_sec * 1000000 + start->tv_usec;
    tstop = (uint64_t)stop->tv_sec * 1000000 + stop->tv_usec;

    bfdev_log_debug(
        "\treal time: %.6lf\n",
        (double)(tstop - tstart) / 1000000
    );

    bfdev_log_debug(
        "\tuser time: %.3lf\n",
        (etms->tms_utime - stms->tms_utime) / ticks
    );

    bfdev_log_debug(
        "\tkern time: %.3lf\n",
        (etms->tms_stime - stms->tms_stime) / ticks
    );
}

#define EXAMPLE_TIME_STATISTICAL(codeblock...) ({   \
    struct tms __start_tms, __stop_tms;             \
    struct timeval __start_time, __stop_time;       \
    unsigned int __ticks;                           \
    int __retval;                                   \
                                                    \
    __ticks = sysconf(_SC_CLK_TCK);                 \
    times(&__start_tms);                            \
    gettimeofday(&__start_time, NULL);              \
                                                    \
    __retval = ({                                   \
        codeblock                                   \
    });                                             \
                                                    \
    gettimeofday(&__stop_time, NULL);               \
    times(&__stop_tms);                             \
                                                    \
    time_dump(                                      \
        (double)__ticks,                            \
        &__start_tms, &__stop_tms,                  \
        &__start_time, &__stop_time                 \
    );                                              \
                                                    \
    __retval;                                       \
})

#define EXAMPLE_TIME_LOOP(loop, time, codeblock...) ({  \
    struct timeval __curr_time, __stop_time;            \
    int __retval;                                       \
                                                        \
    gettimeofday(&__stop_time, NULL);                   \
    __stop_time.tv_sec += (time) / 1000;                \
    __stop_time.tv_usec += ((time) % 1000) * 1000;      \
    *(loop) = 0;                                        \
                                                        \
    do {                                                \
        __retval = ({                                   \
            codeblock                                   \
        });                                             \
                                                        \
        if (__retval)                                   \
            break;                                      \
                                                        \
        ++*(loop);                                      \
        gettimeofday(&__curr_time, NULL);               \
    } while (timercmp(&__curr_time, &__stop_time, <));  \
                                                        \
    __retval;                                           \
})

#endif /* _EXAMPLES_TIME_H_ */
