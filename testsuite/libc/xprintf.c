/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "libc-xprintf"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/xprintf.h>
#include <bfdev/log.h>
#include <testsuite.h>

struct {
    const char *fmt;
    int val;
    const char *expect;
} int_tests[] = {
    /* width, precision, alignment */
    { "%04d", 12, "0012" },
    { "%.3d", 12, "012" },
    { "%3d", 12, " 12" },
    { "%-3d", 12, "12 " },
    { "%+3d", 12, "+12" },
    { "%+-5d", 12, "+12  " },
    { "%+- 5d", 12, "+12  " },
    { "%- 5d", 12, " 12  " },
    { "% d", 12, " 12" },
    { "%0-5d", 12, "12   " },
    { "%-05d", 12, "12   " },

    /* ...explicit precision of 0 shall be no characters except for alt-octal. */
    { "%.0d", 0, "" },
    { "%.0o", 0, "" },
    { "%#.0d", 0, "" },
    { "%#.0o", 0, "0" },
    { "%#.0x", 0, "" },

    /* ...but it still has to honor width and flags. */
    { "%2.0u", 0, "  " },
    { "%02.0u", 0, "  " },
    { "%2.0d", 0, "  " },
    { "%02.0d", 0, "  " },
    { "% .0d", 0, " " },
    { "%+.0d", 0, "+" },

    /* hex: test alt form and case */
    { "%x", 63, "3f" },
    { "%#x", 63, "0x3f" },
    { "%X", 63, "3F" },

    /* octal: test alt form */
    { "%o", 15, "17" },
    { "%#o", 15, "017" },

    /* octal: corner cases */
    { "%#o", 0, "0" },
    { "%#.0o", 0, "0" },
    { "%#.1o", 0, "0" },
    { "%#o", 1, "01" },
    { "%#.0o", 1, "01" },
    { "%#.1o", 1, "01" },
    { "%#04o", 1, "0001" },
    { "%#04.0o", 1, "  01" },
    { "%#04.1o", 1, "  01" },
    { "%04o", 1, "0001" },
    { "%04.0o", 1, "   1" },
    { "%04.1o", 1, "   1" },

    { }, /* NULL */
};

TESTSUITE(
    "xprintf:intfmt",
    NULL, NULL,
    "xprintf int format test"
) {
    unsigned int count;
    char buff[256];
    int retval;

    for (count = 0; int_tests[count].fmt; ++count) {
        bfdev_log_info("test%u: fotmat '%s'\n", count, int_tests[count].fmt);
        retval = bfdev_xprintf(buff, sizeof(buff), int_tests[count].fmt,
            int_tests[count].val);

        if (retval != strlen(int_tests[count].expect)) {
            bfdev_log_err("test%u: bad length got %d, want %d\n",
                count, retval, (int)strlen(int_tests[count].expect));
            return -BFDEV_EFAULT;
        }

        if (strcmp(buff, int_tests[count].expect) != 0) {
            bfdev_log_err("test%u: bad format got '%s', want '%s'\n",
                count, buff, int_tests[count].expect);
            return -BFDEV_EFAULT;
        }
    }

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "xprintf:misc",
    NULL, NULL,
    "xprintf misc test"
) {
    char buff[64];
    int retval;

    if (bfdev_xprintf(NULL, 0, "%d", 123456) != 6)
        return -BFDEV_EFAULT;

    if (bfdev_xprintf(NULL, 0, "%.4s", "hello") != 4)
        return -BFDEV_EFAULT;

    if (bfdev_xprintf(buff, 0, "%.0s", "goodbye") != 0)
        return -BFDEV_EFAULT;

    retval = bfdev_xprintf(buff, 8, "%s", "helloworld");
    if (retval != 10 || strcmp(buff, "hellowo"))
        return -BFDEV_EFAULT;

    retval = bfdev_xprintf(buff, 8, "%*s", 8, "foo");
    if (retval != 8 || strcmp(buff, "     fo"))
        return -BFDEV_EFAULT;

    retval = bfdev_xprintf(buff, 8, "%*s", -8, "bar");
    if (retval != 8 || strcmp(buff, "bar    "))
        return -BFDEV_EFAULT;

    retval = bfdev_xprintf(buff, sizeof(buff), "%c", 'x');
    if (retval != 1 || strcmp(buff, "x"))
        return -BFDEV_EFAULT;

    retval = bfdev_xprintf(buff, sizeof(buff), "%p", (void *)0xdeadbeef);
    if ((!(retval == 10 && !strcmp(buff, "0xdeadbeef")) &&
        (!(retval == 18 && !strcmp(buff, "0x00000000deadbeef")))))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}
