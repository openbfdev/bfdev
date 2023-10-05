/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "crc-bandwidth"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <bfdev/crc.h>
#include <bfdev/bits.h>
#include <bfdev/log.h>
#include <bfdev/size.h>

#define TEST_SIZE BFDEV_SZ_1MiB
#define TEST_LOOP 3

#define GENERIC_CRC_BANDWIDTH(func, name, size)         \
for (count = 0; count < TEST_LOOP; ++count) {           \
    gettimeofday(&etval, NULL);                         \
    etval.tv_sec++;                                     \
    loop = 0;                                           \
                                                        \
    do {                                                \
        func(buff, size, 0);                            \
        loop++;                                         \
        gettimeofday(&stval, NULL);                     \
    } while (timercmp(&stval, &etval, <));              \
    bfdev_log_info(name " bandwidth %u: %uMiB/s\n",     \
                   count, loop);                        \
}

int main(int argc, char const *argv[])
{
    struct timeval stval, etval;
    unsigned int count, loop;
    uint8_t *buff;
    size_t index;

    buff = malloc(TEST_SIZE);
    if (!buff)
        return 1;

    srand(time(NULL));
    for (index = 0; index < TEST_SIZE; ++index)
        buff[index] = (uint8_t)rand();

    GENERIC_CRC_BANDWIDTH(bfdev_crc4, "crc4", TEST_SIZE * BFDEV_BITS_PER_BYTE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc7, "crc7", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc8, "crc8", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc16, "crc16", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc32, "crc32", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc64, "crc64", TEST_SIZE)

    GENERIC_CRC_BANDWIDTH(bfdev_crc_ccitt, "crc-ccitt", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc_itut, "crc-itut", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc_t10dif, "crc-t10dif", TEST_SIZE)
    GENERIC_CRC_BANDWIDTH(bfdev_crc_rocksoft, "crc-rocksoft", TEST_SIZE)

    free(buff);
    return 0;
}
