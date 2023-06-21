/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/fifo.h>

#define TEST_LOOP 16

struct test_pdata {
    BFDEV_DECLARE_FIFO(normal_bytetest, char, TEST_LOOP);
    BFDEV_DECLARE_FIFO(normal_longtest, long, TEST_LOOP);
    BFDEV_DECLARE_FIFO_RECORD(record_bytetest, char, TEST_LOOP, 1);
    BFDEV_DECLARE_FIFO_RECORD(record_longtest, long, TEST_LOOP, 1);
    BFDEV_DECLARE_FIFO_DYNAMIC(dynamic_bytetest, char);
    BFDEV_DECLARE_FIFO_DYNAMIC(dynamic_longtest, long);
    BFDEV_DECLARE_FIFO_DYNAMIC_RECORD(dynamic_record_bytetest, char, 1);
    BFDEV_DECLARE_FIFO_DYNAMIC_RECORD(dynamic_record_longtest, long, 1);
};

static const char bytetest_table[TEST_LOOP] = {
    'o', 'p', 'e', 'n', 'b', 'f', 'd', 'e',
    'v', ',', 'h', 'e', 'l', 'l', 'l', 'o',
};

static const long longtest_table[TEST_LOOP] = {
    (long)0x0000000000000000ULL, (long)0x1111111111111111ULL,
    (long)0x2222222222222222ULL, (long)0x3333333333333333ULL,
    (long)0x4444444444444444ULL, (long)0x5555555555555555ULL,
    (long)0x6666666666666666ULL, (long)0x7777777777777777ULL,
    (long)0x8888888888888888ULL, (long)0x9999999999999999ULL,
    (long)0xaaaaaaaaaaaaaaaaULL, (long)0xbbbbbbbbbbbbbbbbULL,
    (long)0xccccccccccccccccULL, (long)0xddddddddddddddddULL,
    (long)0xeeeeeeeeeeeeeeeeULL, (long)0xffffffffffffffffULL,
};

static int fifo_testing(struct test_pdata *pdata)
{
    char bytevalue[TEST_LOOP];
    long longvalue[TEST_LOOP];
    unsigned int count;
    unsigned long retval;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_fifo_put(&pdata->normal_bytetest, bytetest_table[count]);
        printf("fifo normal_bytetest %u put '%c': ", count, bytetest_table[count]);
        if (!retval || bfdev_fifo_len(&pdata->normal_bytetest) != count + 1) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo normal_bytetest check full: ");
    if (!bfdev_fifo_check_full(&pdata->normal_bytetest)) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_fifo_peek(&pdata->normal_bytetest, bytevalue);
        printf("fifo normal_bytetest %u peek '%c': ", count, *bytevalue);
        if (!retval || *bytevalue != bytetest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        retval = bfdev_fifo_get(&pdata->normal_bytetest, bytevalue);
        printf("fifo normal_bytetest %u get '%c': ", count, *bytevalue);
        if (!retval || *bytevalue != bytetest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo normal_bytetest copy in: ");
    retval = bfdev_fifo_in(&pdata->normal_bytetest, bytetest_table, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_bytetest copy peek out: ");
    retval = bfdev_fifo_out_peek(&pdata->normal_bytetest, bytevalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_bytetest check peek out: ");
    if (memcmp(bytevalue, bytetest_table, sizeof(bytetest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_bytetest copy out: ");
    retval = bfdev_fifo_out(&pdata->normal_bytetest, bytevalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_bytetest check out: ");
    if (memcmp(bytevalue, bytetest_table, sizeof(bytetest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_fifo_put(&pdata->normal_longtest, longtest_table[count]);
        printf("fifo normal_longtest %u put %#lx: ", count, longtest_table[count]);
        if (bfdev_fifo_len(&pdata->normal_longtest) != count + 1) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo normal_longtest check full: ");
    if (!bfdev_fifo_check_full(&pdata->normal_longtest)) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_fifo_peek(&pdata->normal_longtest, longvalue);
        printf("fifo normal_longtest %u peek %#lx: ", count, *longvalue);
        if (!retval || *longvalue != longtest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        retval = bfdev_fifo_get(&pdata->normal_longtest, longvalue);
        printf("fifo normal_longtest %u get %#lx: ", count, *longvalue);
        if (!retval || *longvalue != longtest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo normal_longtest copy in: ");
    retval = bfdev_fifo_in(&pdata->normal_longtest, longtest_table, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_longtest copy peek out: ");
    retval = bfdev_fifo_out_peek(&pdata->normal_longtest, longvalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_longtest check peek out: ");
    if (memcmp(longvalue, longtest_table, sizeof(longtest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_longtest copy out: ");
    retval = bfdev_fifo_out(&pdata->normal_longtest, longvalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_longtest check out: ");
    if (memcmp(longvalue, longtest_table, sizeof(longtest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_fifo_put(&pdata->dynamic_bytetest, bytetest_table[count]);
        printf("fifo dynamic_bytetest %u put '%c': ", count, bytetest_table[count]);
        if (!retval || bfdev_fifo_len(&pdata->dynamic_bytetest) != count + 1) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo dynamic_bytetest check full: ");
    if (!bfdev_fifo_check_full(&pdata->dynamic_bytetest)) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_fifo_peek(&pdata->dynamic_bytetest, bytevalue);
        printf("fifo dynamic_bytetest %u peek '%c': ", count, *bytevalue);
        if (!retval || *bytevalue != bytetest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        retval = bfdev_fifo_get(&pdata->dynamic_bytetest, bytevalue);
        printf("fifo dynamic_bytetest %u get '%c': ", count, *bytevalue);
        if (!retval || *bytevalue != bytetest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo dynamic_bytetest copy in: ");
    retval = bfdev_fifo_in(&pdata->dynamic_bytetest, bytetest_table, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo dynamic_bytetest copy peek out: ");
    retval = bfdev_fifo_out_peek(&pdata->dynamic_bytetest, bytevalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo dynamic_bytetest check peek out: ");
    if (memcmp(bytetest_table, bytetest_table, sizeof(bytetest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo dynamic_bytetest copy out: ");
    retval = bfdev_fifo_out(&pdata->dynamic_bytetest, bytevalue, TEST_LOOP);
    if (retval != TEST_LOOP)
        return 1;
    printf("pass\n");

    printf("fifo dynamic_bytetest check out: ");
    if (memcmp(bytevalue, bytetest_table, sizeof(bytetest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_fifo_put(&pdata->dynamic_longtest, longtest_table[count]);
        printf("fifo dynamic_longtest %u put %#lx: ", count, longtest_table[count]);
        if (bfdev_fifo_len(&pdata->dynamic_longtest) != count + 1) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo dynamic_longtest check full: ");
    if (!bfdev_fifo_check_full(&pdata->dynamic_longtest)) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_fifo_peek(&pdata->dynamic_longtest, longvalue);
        printf("fifo dynamic_longtest %u peek %#lx: ", count, *longvalue);
        if (!retval || *longvalue != longtest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        retval = bfdev_fifo_get(&pdata->dynamic_longtest, longvalue);
        printf("fifo dynamic_longtest %u get %#lx: ", count, *longvalue);
        if (!retval || *longvalue != longtest_table[count]) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    printf("fifo dynamic_longtest copy in: ");
    retval = bfdev_fifo_in(&pdata->dynamic_longtest, longtest_table, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo dynamic_longtest copy peek out: ");
    retval = bfdev_fifo_out_peek(&pdata->dynamic_longtest, longvalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo dynamic_longtest check peek out: ");
    if (memcmp(longvalue, longtest_table, sizeof(longtest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo dynamic_longtest copy out: ");
    retval = bfdev_fifo_out(&pdata->dynamic_longtest, longvalue, TEST_LOOP);
    if (retval != TEST_LOOP) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    printf("fifo normal_longtest check out: ");
    if (memcmp(longvalue, longtest_table, sizeof(longtest_table))) {
        printf("failed\n");
        return 1;
    }
    printf("pass\n");

    for (count = 1; count < TEST_LOOP; ++count) {
        printf("fifo record_bytetest copy %u in: ", count);
        retval = bfdev_fifo_in(&pdata->record_bytetest, bytetest_table, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo record_bytetest copy %u out: ", count);
        retval = bfdev_fifo_out(&pdata->record_bytetest, bytevalue, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo record_bytetest check %u copy: ", count);
        if (memcmp(bytevalue, bytetest_table, count)) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    for (count = 1; count < TEST_LOOP; ++count) {
        printf("fifo record_longtest copy %u in: ", count);
        retval = bfdev_fifo_in(&pdata->record_longtest, longtest_table, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo record_longtest copy %u out: ", count);
        retval = bfdev_fifo_out(&pdata->record_longtest, longvalue, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo record_longtest check %u copy: ", count);
        if (memcmp(longvalue, longtest_table, count)) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    for (count = 1; count < TEST_LOOP; ++count) {
        printf("fifo dynamic_record_bytetest copy %u in: ", count);
        retval = bfdev_fifo_in(&pdata->dynamic_record_bytetest, bytetest_table, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo dynamic_record_bytetest copy %u out: ", count);
        retval = bfdev_fifo_out(&pdata->dynamic_record_bytetest, bytevalue, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo dynamic_record_bytetest check %u copy: ", count);
        if (memcmp(bytevalue, bytetest_table, count)) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    for (count = 1; count < TEST_LOOP; ++count) {
        printf("fifo dynamic_record_longtest copy %u in: ", count);
        retval = bfdev_fifo_in(&pdata->dynamic_record_longtest, longtest_table, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo dynamic_record_longtest copy %u out: ", count);
        retval = bfdev_fifo_out(&pdata->dynamic_record_longtest, longvalue, count);
        if (retval != count) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");

        printf("fifo dynamic_record_longtest check %u copy: ", count);
        if (memcmp(longvalue, longtest_table, count)) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    return 0;
}

int main(void)
{
    struct test_pdata *pdata;
    int retval;

    pdata = malloc(sizeof(struct test_pdata));
    if (!pdata)
        return 1;

    pdata->normal_bytetest = BFDEV_FIFO_INIT(pdata->normal_bytetest);
    pdata->normal_longtest = BFDEV_FIFO_INIT(pdata->normal_longtest);
    pdata->record_bytetest = BFDEV_FIFO_INIT(pdata->record_bytetest);
    pdata->record_longtest = BFDEV_FIFO_INIT(pdata->record_longtest);

    if ((retval = bfdev_fifo_alloc(&pdata->dynamic_bytetest, NULL, TEST_LOOP)) ||
        (retval = bfdev_fifo_alloc(&pdata->dynamic_longtest, NULL, TEST_LOOP)) ||
        (retval = bfdev_fifo_alloc(&pdata->dynamic_record_bytetest, NULL, TEST_LOOP)) ||
        (retval = bfdev_fifo_alloc(&pdata->dynamic_record_longtest, NULL, TEST_LOOP)))
        return 1;

    retval = fifo_testing(pdata);
    if (retval)
        return retval;

    bfdev_fifo_free(&pdata->dynamic_bytetest);
    bfdev_fifo_free(&pdata->dynamic_longtest);
    bfdev_fifo_free(&pdata->dynamic_record_bytetest);
    bfdev_fifo_free(&pdata->dynamic_record_longtest);
    free(pdata);

    return 0;
}
