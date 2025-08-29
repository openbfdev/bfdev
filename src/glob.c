/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/glob.h>
#include <export.h>

export bfdev_bool
bfdev_glob(const char *pattern, const char *string)
{
    const char *class, *bpatten, *bstring;
    char ptch, stch, tcha, tchb;
    bfdev_bool match, inverted;

    bpatten = BFDEV_NULL;
    bstring = BFDEV_NULL;

    for (;;) {
        ptch = *pattern++;
        stch = *string++;

        switch (ptch) {
            case '?':
                if (stch == '\0')
                    return bfdev_false;
                break;

            case '*':
                if (*pattern == '\0')
                    return bfdev_true;
                bpatten = pattern;
                bstring = --string;
                break;

            case '[':
                match = bfdev_false;
                inverted = *pattern == '!';
                class = pattern + inverted;
                tcha = *class++;

                do {
                    tchb = tcha;
                    if (tcha == '\0')
                        goto literal;

                    if (class[0] == '-' && class[1] != ']') {
                        tchb = class[1];
                        if (tchb == '\0')
                            goto literal;
                        class += 2;
                    }
                    match |= tcha <= stch && stch <= tchb;
                } while ((tcha = *class++) != ']');

                if (match == inverted)
                    goto backtrack;

                pattern = class;
                break;

            case '\\':
                ptch = *pattern++;
                bfdev_fallthrough;

            default: literal:
                if (ptch == stch) {
                    if (ptch == '\0')
                        return bfdev_true;
                    break;
                }

            backtrack:
                if (stch == '\0' || !bpatten)
                    return bfdev_false;

                pattern = bpatten;
                string = ++bstring;
                break;
        }
    }
}
