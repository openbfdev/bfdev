/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <bfdev/log.h>

static inline void
time_dump(double ticks, clock_t start, clock_t end, struct tms *stms, struct tms *etms)
{
    bfdev_log_info("\treal time: %lf\n", (end - start) / ticks);
    bfdev_log_info("\tuser time: %lf\n", (etms->tms_utime - stms->tms_utime) / ticks);
    bfdev_log_info("\tkern time: %lf\n", (etms->tms_stime - stms->tms_stime) / ticks);
}

#define EXAMPLE_TIME_STATISTICAL(codeblock...) ({   \
    struct tms start_tms, stop_tms;                 \
    clock_t start, stop;                            \
    unsigned int ticks;                             \
    int retval;                                     \
                                                    \
    ticks = sysconf(_SC_CLK_TCK);                   \
    start = times(&start_tms);                      \
                                                    \
    retval = ({                                     \
        codeblock                                   \
    });                                             \
                                                    \
    stop = times(&stop_tms);                        \
    time_dump((double)ticks, start, stop,           \
              &start_tms, &stop_tms);               \
                                                    \
    retval;                                         \
})

#define EXAMPLE_TIME_LOOP(loop, time, codeblock...) ({  \
    struct timeval curr_timval, stop_timval;            \
    int retval;                                         \
                                                        \
    gettimeofday(&stop_timval, NULL);                   \
    stop_timval.tv_sec += (time) / 1000;                \
    stop_timval.tv_usec += ((time) % 1000) * 1000;      \
    *(loop) = 0;                                        \
                                                        \
    do {                                                \
        retval = ({                                     \
            codeblock                                   \
        });                                             \
                                                        \
        if (retval)                                     \
            break;                                      \
                                                        \
        ++*(loop);                                      \
        gettimeofday(&curr_timval, NULL);               \
    } while (timercmp(&curr_timval, &stop_timval, <));  \
                                                        \
    retval;                                             \
})
