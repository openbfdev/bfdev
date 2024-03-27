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
time_dump(double ticks, clock_t start, clock_t end,
          struct tms *stms, struct tms *etms)
{
    bfdev_log_debug(
        "\treal time: %lf\n",
        (end - start) / ticks
    );

    bfdev_log_debug(
        "\tuser time: %lf\n",
        (etms->tms_utime - stms->tms_utime) / ticks
    );

    bfdev_log_debug(
        "\tkern time: %lf\n",
        (etms->tms_stime - stms->tms_stime) / ticks
    );
}

#define EXAMPLE_TIME_STATISTICAL(codeblock...) ({   \
    struct tms _start_tms, _stop_tms;               \
    clock_t _start, _stop;                          \
    unsigned int _ticks;                            \
    int _retval;                                    \
                                                    \
    _ticks = sysconf(_SC_CLK_TCK);                  \
    _start = times(&_start_tms);                    \
                                                    \
    _retval = ({                                    \
        codeblock                                   \
    });                                             \
                                                    \
    _stop = times(&_stop_tms);                      \
    time_dump((double)_ticks, _start, _stop,        \
              &_start_tms, &_stop_tms);             \
                                                    \
    _retval;                                        \
})

#define EXAMPLE_TIME_LOOP(loop, time, codeblock...) ({      \
    struct timeval _curr_timval, _stop_timval;              \
    int _retval;                                            \
                                                            \
    gettimeofday(&_stop_timval, NULL);                      \
    _stop_timval.tv_sec += (time) / 1000;                   \
    _stop_timval.tv_usec += ((time) % 1000) * 1000;         \
    *(loop) = 0;                                            \
                                                            \
    do {                                                    \
        _retval = ({                                        \
            codeblock                                       \
        });                                                 \
                                                            \
        if (_retval)                                        \
            break;                                          \
                                                            \
        ++*(loop);                                          \
        gettimeofday(&_curr_timval, NULL);                  \
    } while (timercmp(&_curr_timval, &_stop_timval, <));    \
                                                            \
    _retval;                                                \
})

#endif /* _EXAMPLES_TIME_H_ */
