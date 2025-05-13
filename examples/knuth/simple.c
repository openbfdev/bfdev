/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "knuth-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdlib.h>
#include <time.h>
#include <bfdev/macro.h>
#include <bfdev/log.h>
#include <bfdev/knuth.h>

static const char *
cards[12] = {
    "Ace", "2", "3", "4", "5", "6", "7",
    "8", "9", "Jack", "Queen", "King"
};

int
main(int argc, const char *argv[])
{
    unsigned int index;

    srand(time(NULL));
    bfdev_knuth(cards, BFDEV_ARRAY_SIZE(cards), sizeof(*cards));
    for (index = 0; index < BFDEV_ARRAY_SIZE(cards); ++index)
        bfdev_log_info("card%u: %s\n", index, cards[index]);

    return 0;
}
