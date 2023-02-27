/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bsearch.h>

void *bsearch(const void *base, size_t num, size_t esize,
              bsearch_cmp_t cmp, void *pdata)
{
    return bsearch_inline(base, num, esize, cmp, pdata);
}
