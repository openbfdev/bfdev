/* SPDX-License-Identifier\nGPL-2.0-or-later */
/*
 * Copyright(c) 2022-2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bitwalk-selftest"
#define bfdev_log_fmt(fmt) MODULE_NAME ":" fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/fifo.h>
#include <bfdev/log.h>
#include <testsuite.h>

#define TEST_LOOP 16

static const char
bytetest_table[TEST_LOOP] = {
    'o', 'p', 'e', 'n', 'b', 'f', 'd', 'e',
    'v', ',', 'h', 'e', 'l', 'l', 'l', 'o',
};

static const long
longtest_table[TEST_LOOP] = {
    BFDEV_REPEAT_BYTE(0x00), BFDEV_REPEAT_BYTE(0x11),
    BFDEV_REPEAT_BYTE(0x22), BFDEV_REPEAT_BYTE(0x33),
    BFDEV_REPEAT_BYTE(0x44), BFDEV_REPEAT_BYTE(0x55),
    BFDEV_REPEAT_BYTE(0x66), BFDEV_REPEAT_BYTE(0x77),
    BFDEV_REPEAT_BYTE(0x88), BFDEV_REPEAT_BYTE(0x99),
    BFDEV_REPEAT_BYTE(0xaa), BFDEV_REPEAT_BYTE(0xbb),
    BFDEV_REPEAT_BYTE(0xcc), BFDEV_REPEAT_BYTE(0xdd),
    BFDEV_REPEAT_BYTE(0xee), BFDEV_REPEAT_BYTE(0xff),
};

TESTSUITE(
    "fifo:bytetest", NULL, NULL,
    "fifo bytetest"
) {
    BFDEV_DECLARE_FIFO(fifo, char, TEST_LOOP);
    char buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_INIT(&fifo);
    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_put(&fifo, bytetest_table[count]);
        bfdev_log_debug("bytetest %u put '%c'\n", count, bytetest_table[count]);
        if (!length || bfdev_fifo_len(&fifo) != count + 1)
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("bytetest check full\n");
    if (!bfdev_fifo_check_full(&fifo))
        return -BFDEV_EFAULT;

    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_peek(&fifo, buffer);
        bfdev_log_debug("bytetest %u peek '%c'\n", count, *buffer);
        if (!length || *buffer != bytetest_table[count])
            return -BFDEV_EFAULT;

        length = bfdev_fifo_get(&fifo, buffer);
        bfdev_log_debug("bytetest %u get '%c'\n", count, *buffer);
        if (!length || *buffer != bytetest_table[count])
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("bytetest copy in\n");
    length = bfdev_fifo_in(&fifo, bytetest_table, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("bytetest copy peek out\n");
    length = bfdev_fifo_out_peek(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("bytetest check peek out\n");
    retval = memcmp(buffer, bytetest_table, sizeof(bytetest_table));
    if (retval)
        return -BFDEV_EFAULT;

    bfdev_log_debug("bytetest copy out\n");
    length = bfdev_fifo_out(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("bytetest check out\n");
    retval = memcmp(buffer, bytetest_table, sizeof(bytetest_table));
    if (retval)
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:longtest", NULL, NULL,
    "fifo longtest"
) {
    BFDEV_DECLARE_FIFO(fifo, long, TEST_LOOP);
    long buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_INIT(&fifo);
    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_put(&fifo, longtest_table[count]);
        bfdev_log_debug("longtest %u put %#lx\n", count, longtest_table[count]);
        if (!length || bfdev_fifo_len(&fifo) != count + 1)
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("longtest check full\n");
    if (!bfdev_fifo_check_full(&fifo))
        return -BFDEV_EFAULT;

    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_peek(&fifo, buffer);
        bfdev_log_debug("longtest %u peek %#lx\n", count, *buffer);
        if (!length || *buffer != longtest_table[count])
            return -BFDEV_EFAULT;

        length = bfdev_fifo_get(&fifo, buffer);
        bfdev_log_debug("longtest %u get %#lx\n", count, *buffer);
        if (!length || *buffer != longtest_table[count])
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("longtest copy in\n");
    length = bfdev_fifo_in(&fifo, longtest_table, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("longtest copy peek out\n");
    length = bfdev_fifo_out_peek(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("longtest check peek out\n");
    retval = memcmp(buffer, longtest_table, sizeof(longtest_table));
    if (retval)
        return -BFDEV_EFAULT;

    bfdev_log_debug("longtest copy out\n");
    length = bfdev_fifo_out(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("longtest check out\n");
    retval = memcmp(buffer, longtest_table, sizeof(longtest_table));
    if (retval)
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:record_bytetest", NULL, NULL,
    "fifo record bytetest"
) {
    BFDEV_DECLARE_FIFO_RECORD(fifo, char, TEST_LOOP, 1);
    char buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_INIT(&fifo);
    for (count = 1; count < TEST_LOOP; ++count) {
        bfdev_log_debug("record bytetest copy %u in\n", count);
        length = bfdev_fifo_in(&fifo, bytetest_table, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record bytetest copy %u out\n", count);
        length = bfdev_fifo_out(&fifo, buffer, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record bytetest check %u copy\n", count);
        retval = memcmp(buffer, bytetest_table, count);
        if (retval)
            return -BFDEV_EFAULT;
    }

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:record_longtest", NULL, NULL,
    "fifo record longtest"
) {
    BFDEV_DECLARE_FIFO_RECORD(fifo, long, TEST_LOOP, 1);
    long buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_INIT(&fifo);
    for (count = 1; count < TEST_LOOP; ++count) {
        bfdev_log_debug("record longtest copy %u in\n", count);
        length = bfdev_fifo_in(&fifo, longtest_table, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record longtest copy %u out\n", count);
        length = bfdev_fifo_out(&fifo, buffer, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record longtest check %u copy\n", count);
        retval = memcmp(buffer, longtest_table, count);
        if (retval)
            return -BFDEV_EFAULT;
    }

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:dynamic_bytetest", NULL, NULL,
    "fifo dynamic bytetest"
) {
    BFDEV_DECLARE_FIFO_DYNAMIC(fifo, char);
    char buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_DYNAMIC_INIT(&fifo);
    retval = bfdev_fifo_alloc(&fifo, NULL, TEST_LOOP);
    if (retval)
        return retval;

    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_put(&fifo, bytetest_table[count]);
        bfdev_log_debug("dynamic bytetest %u put '%c'\n", count, bytetest_table[count]);
        if (!length || bfdev_fifo_len(&fifo) != count + 1)
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("dynamic bytetest check full\n");
    if (!bfdev_fifo_check_full(&fifo))
        return -BFDEV_EFAULT;

    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_peek(&fifo, buffer);
        bfdev_log_debug("dynamic bytetest %u peek '%c'\n", count, *buffer);
        if (!length || *buffer != bytetest_table[count])
            return -BFDEV_EFAULT;

        length = bfdev_fifo_get(&fifo, buffer);
        bfdev_log_debug("dynamic bytetest %u get '%c'\n", count, *buffer);
        if (!length || *buffer != bytetest_table[count])
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("dynamic bytetest copy in\n");
    length = bfdev_fifo_in(&fifo, bytetest_table, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic bytetest copy peek out\n");
    length = bfdev_fifo_out_peek(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic bytetest check peek out\n");
    retval = memcmp(bytetest_table, bytetest_table, sizeof(bytetest_table));
    if (retval)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic bytetest copy out\n");
    length = bfdev_fifo_out(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic bytetest check out\n");
    retval = memcmp(buffer, bytetest_table, sizeof(bytetest_table));
    if (retval)
        return -BFDEV_EFAULT;

    bfdev_fifo_free(&fifo);

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:dynamic_longtest", NULL, NULL,
    "fifo dynamic longtest"
) {
    BFDEV_DECLARE_FIFO_DYNAMIC(fifo, long);
    long buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_DYNAMIC_INIT(&fifo);
    retval = bfdev_fifo_alloc(&fifo, NULL, TEST_LOOP);
    if (retval)
        return retval;

    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_put(&fifo, longtest_table[count]);
        bfdev_log_debug("longtest %u put %#lx\n", count, longtest_table[count]);
        if (!length || bfdev_fifo_len(&fifo) != count + 1)
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("dynamic longtest check full\n");
    if (!bfdev_fifo_check_full(&fifo))
        return -BFDEV_EFAULT;

    for (count = 0; count < TEST_LOOP; ++count) {
        length = bfdev_fifo_peek(&fifo, buffer);
        bfdev_log_debug("dynamic longtest %u peek %#lx\n", count, *buffer);
        if (!length || *buffer != longtest_table[count])
            return -BFDEV_EFAULT;

        length = bfdev_fifo_get(&fifo, buffer);
        bfdev_log_debug("dynamic longtest %u get %#lx\n", count, *buffer);
        if (!length || *buffer != longtest_table[count])
            return -BFDEV_EFAULT;
    }

    bfdev_log_debug("dynamic longtest copy in\n");
    length = bfdev_fifo_in(&fifo, longtest_table, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic longtest copy peek out\n");
    length = bfdev_fifo_out_peek(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic longtest check peek out\n");
    retval = memcmp(buffer, longtest_table, sizeof(longtest_table));
    if (retval)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic longtest copy out\n");
    length = bfdev_fifo_out(&fifo, buffer, TEST_LOOP);
    if (length != TEST_LOOP)
        return -BFDEV_EFAULT;

    bfdev_log_debug("dynamic longtest check out\n");
    retval = memcmp(buffer, longtest_table, sizeof(longtest_table));
    if (retval)
        return -BFDEV_EFAULT;

    bfdev_fifo_free(&fifo);

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:dynamic_record_bytetest", NULL, NULL,
    "fifo dynamic_record bytetest"
) {
    BFDEV_DECLARE_FIFO_DYNAMIC_RECORD(fifo, char, 1);
    char buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_DYNAMIC_INIT(&fifo);
    retval = bfdev_fifo_alloc(&fifo, NULL, TEST_LOOP);
    if (retval)
        return retval;

    for (count = 1; count < TEST_LOOP; ++count) {
        bfdev_log_debug("record bytetest copy %u in\n", count);
        length = bfdev_fifo_in(&fifo, bytetest_table, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record bytetest copy %u out\n", count);
        length = bfdev_fifo_out(&fifo, buffer, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record bytetest check %u copy\n", count);
        retval = memcmp(buffer, bytetest_table, count);
        if (retval)
            return -BFDEV_EFAULT;
    }

    bfdev_fifo_free(&fifo);

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "fifo:dynamic_record_longtest", NULL, NULL,
    "fifo dynamic record longtest"
) {
    BFDEV_DECLARE_FIFO_DYNAMIC_RECORD(fifo, long, 1);
    long buffer[TEST_LOOP];
    unsigned long length;
    unsigned int count;
    int retval;

    fifo = BFDEV_FIFO_DYNAMIC_INIT(&fifo);
    retval = bfdev_fifo_alloc(&fifo, NULL, TEST_LOOP);
    if (retval)
        return retval;

    for (count = 1; count < TEST_LOOP; ++count) {
        bfdev_log_debug("record longtest copy %u in\n", count);
        length = bfdev_fifo_in(&fifo, longtest_table, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record longtest copy %u out\n", count);
        length = bfdev_fifo_out(&fifo, buffer, count);
        if (length != count)
            return -BFDEV_EFAULT;

        bfdev_log_debug("record longtest check %u copy\n", count);
        retval = memcmp(buffer, longtest_table, count);
        if (retval)
            return -BFDEV_EFAULT;
    }

    bfdev_fifo_free(&fifo);

    return -BFDEV_ENOERR;
}
