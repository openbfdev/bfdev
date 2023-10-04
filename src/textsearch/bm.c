/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <textsearch.h>

struct bm_context {
    struct bfdev_ts_context tsc;
    uint8_t *pattern;
    unsigned int pattern_len;
    unsigned int bad_shift[UINT8_MAX];
    unsigned int good_shift[0];
};

#define ts_to_bm(ptr) \
    bfdev_container_of(ptr, struct bm_context, tsc)

static const void *
bm_pattern_get(struct bfdev_ts_context *tsc)
{
    struct bm_context *bctx = ts_to_bm(tsc);
    return bctx->pattern;
}

static unsigned int
bm_pattern_len(struct bfdev_ts_context *tsc)
{
    struct bm_context *bctx = ts_to_bm(tsc);
    return bctx->pattern_len;
}

static unsigned int
bm_find(struct bfdev_ts_context *tsc, struct bfdev_ts_state *tss)
{
    #define find_pattern() (icase ? toupper(text[shift - index]) : text[shift - index])
    bool icase = bfdev_ts_test_igcase(tsc);
    struct bm_context *bctx = ts_to_bm(tsc);
    unsigned int bad_shift, good_shift, consumed = tss->offset;
    unsigned int length, index, shift = bctx->pattern_len - 1;
    const uint8_t *text;

    for (;;) {
        length = tsc->next_block(tsc, tss, consumed, (const void **)&text);
        if (bfdev_unlikely(!length))
            return UINT_MAX;

        while (shift < length) {
            for (index = 0; index < bctx->pattern_len; ++index) {
                if (bctx->pattern[bctx->pattern_len - index - 1] != find_pattern())
                    break;
            }

            if (bfdev_unlikely(index == bctx->pattern_len))
                return consumed + shift - bctx->pattern_len + 1;

            bad_shift = shift - index + bctx->bad_shift[text[shift - index]];
            good_shift = shift + bctx->good_shift[index];
            shift = bfdev_max(bad_shift, good_shift);
        }

        consumed += length;
    }
}

static inline bool
subpattern(const uint8_t *pattern, int index, int j, int g)
{
    int x = index + g - 1, y = j + g - 1;

    while (y && pattern[x--] == pattern[y--]) {
        if (y < 0)
            return true;
        if (!--g)
            return pattern[index - 1] != pattern[j - 1];
    }

    return false;
}

static inline void
compute_prefix(struct bm_context *bctx, unsigned long flags)
{
    int index, start, count;

    for (index = 0; index < UINT8_MAX; ++index)
        bctx->bad_shift[index] = bctx->pattern_len;

    for (index = 0; index < bctx->pattern_len - 1; ++index) {
        bctx->bad_shift[bctx->pattern[index]] = bctx->pattern_len - index - 1;
		if (flags & BFDEV_TS_IGCASE) {
            bctx->bad_shift[tolower(bctx->pattern[index])] =
                bctx->pattern_len - index - 1;
        }
    }

    bctx->good_shift[0] = 1;
    for (index = 1; index < bctx->pattern_len; ++index)
        bctx->good_shift[index] = bctx->pattern_len;

    for (index = bctx->pattern_len - 1, start = 1; index; ++start, --index) {
        for (count = index - 1; count >= 1 - start; --count) {
            if (subpattern(bctx->pattern, index, count, start)) {
                bctx->good_shift[count] = bctx->pattern_len - start - count;
                break;
            }
        }
    }
}

static struct bfdev_ts_context *
bm_prepare(const struct bfdev_alloc *alloc, const void *pattern,
           size_t len, unsigned long flags)
{
    unsigned int gsize = sizeof(unsigned int) * len;
    struct bm_context *bctx;
    unsigned int index;

    bctx = bfdev_malloc(alloc, sizeof(*bctx) + gsize + len);
    if (bfdev_unlikely(!bctx))
        return NULL;

    bctx->tsc.flags = flags;
    bctx->pattern_len = len;
    bctx->pattern = (void *)bctx + sizeof(*bctx) + gsize;

    if (!(flags & BFDEV_TS_IGCASE))
        memcpy(bctx->pattern, pattern, len);
    else for (index = 0; index < len; ++index)
        bctx->pattern[index] = toupper(((char *)pattern)[index]);
    compute_prefix(bctx, flags);

    return &bctx->tsc;
}

static void
bm_destroy(struct bfdev_ts_context *tsc)
{
    struct bm_context *bctx = ts_to_bm(tsc);
    bfdev_free(tsc->alloc, bctx);
}

static struct bfdev_ts_algorithm
bm_algorithm = {
    .name = "bm",
    .find = bm_find,
    .prepare = bm_prepare,
    .destroy = bm_destroy,
    .pattern_get = bm_pattern_get,
    .pattern_len = bm_pattern_len,
};

static __bfdev_ctor int
bm_init(void)
{
    return bfdev_textsearch_register(&bm_algorithm);
}
