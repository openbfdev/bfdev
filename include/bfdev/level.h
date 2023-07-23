/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LEVEL_H_
#define _BFDEV_LEVEL_H_

#include <bfdev/compiler.h>

BFDEV_BEGIN_DECLS

/* ASCII Start Of Header */
#define BFDEV_SOH           "\001"
#define BFDEV_SOH_ASCII     '\001'

#define BFDEV_EMERG         BFDEV_SOH "0"   /* system is unusable */
#define BFDEV_ALERT         BFDEV_SOH "1"   /* action must be taken immediately */
#define BFDEV_CRIT          BFDEV_SOH "2"   /* critical conditions */
#define BFDEV_ERR           BFDEV_SOH "3"   /* error conditions */
#define BFDEV_WARNING       BFDEV_SOH "4"   /* warning conditions */
#define BFDEV_NOTICE        BFDEV_SOH "5"   /* normal but significant condition */
#define BFDEV_INFO          BFDEV_SOH "6"   /* informational */
#define BFDEV_DEBUG         BFDEV_SOH "7"   /* debug-level messages */
#define BFDEV_DEFAULT       ""              /* the default kernel loglevel */

#define BFDEV_LEVEL_EMERG           0
#define BFDEV_LEVEL_ALERT           1
#define BFDEV_LEVEL_CRIT            2
#define BFDEV_LEVEL_ERR             3
#define BFDEV_LEVEL_WARNING         4
#define BFDEV_LEVEL_NOTICE          5
#define BFDEV_LEVEL_INFO            6
#define BFDEV_LEVEL_DEBUG           7
#define BFDEV_LEVEL_DEFAULT         8

#define BFDEV_COLR_DEFAULT          39
#define BFDEV_COLR_BLACK            30
#define BFDEV_COLR_DARK_RED         31
#define BFDEV_COLR_DARK_GREEN       32
#define BFDEV_COLR_DARK_YELLOW      33
#define BFDEV_COLR_DARK_BLUE        34
#define BFDEV_COLR_DARK_MAGENTA     35
#define BFDEV_COLR_DARK_CYAN        36
#define BFDEV_COLR_LIGHT_GRAY       37
#define BFDEV_COLR_DARK_GRAY        90
#define BFDEV_COLR_RED              91
#define BFDEV_COLR_GREEN            92
#define BFDEV_COLR_YELLOW           93
#define BFDEV_COLR_BLUE             94
#define BFDEV_COLR_MAGENTA          95
#define BFDEV_COLR_CYAN             96
#define BFDEV_COLR_WHITE            97

BFDEV_END_DECLS

#endif /* _BFDEV_LEVEL_H_ */
