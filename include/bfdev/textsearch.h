/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TEXTSEARCH_H_
#define _BFDEV_TEXTSEARCH_H_

#include <bfdev/config.h>
#include <bfdev/list.h>
#include <bfdev/bitflags.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_ts_state bfdev_ts_state_t;
typedef struct bfdev_ts_linear bfdev_ts_linear_t;
typedef struct bfdev_ts_context bfdev_ts_context_t;
typedef struct bfdev_ts_algorithm bfdev_ts_algorithm_t;

enum bfdev_ts_flags {
    __BFDEV_TS_IGCASE = 0,
    BFDEV_TS_IGCASE = BFDEV_BIT(__BFDEV_TS_IGCASE),
};

/**
 * struct bfdev_ts_state - Search state.
 * @offset: offset for next match.
 * @offset: search context private data.
 */
struct bfdev_ts_state {
    unsigned int offset;
    void *pdata;
};

struct bfdev_ts_linear {
    bfdev_ts_state_t tss;
    const char *data;
    unsigned int len;
};

/**
 * struct bfdev_ts_context - Search context.
 * @ops: chosen search algorithm.
 * @flags: user flags.
 * @next_block: fetch next block of data.
 */
struct bfdev_ts_context {
    const bfdev_alloc_t *alloc;
    unsigned long flags;

    bfdev_ts_algorithm_t *algo;
    unsigned int (*next_block)(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss,
                               unsigned int consumed, const void **dest);
};

/**
 * struct bfdev_ts_algorithm - Search algorithm operations.
 * @name: name of search algorithm.
 * @prepare: initialization function to prepare a search.
 * @destroy: destroy algorithm specific parts of a search configuration.
 * @find: find the next occurrence of the pattern.
 * @pattern_get: return pointer of pattern.
 * @pattern_len: return length of pattern.
 */
struct bfdev_ts_algorithm {
    bfdev_list_head_t list;
    const char *name;

    bfdev_ts_context_t *(*prepare)(const bfdev_alloc_t *alloc, const void *pattern,
                                   size_t len, unsigned long flags);
    void (*destroy)(bfdev_ts_context_t *tsc);
    unsigned int (*find)(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss);
    const void *(*pattern_get)(bfdev_ts_context_t *tsc);
    unsigned int (*pattern_len)(bfdev_ts_context_t *tsc);
};

BFDEV_BITFLAGS_STRUCT(bfdev_ts, bfdev_ts_context_t, flags)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_ts, bfdev_ts_context_t, flags, igcase, __BFDEV_TS_IGCASE)

static inline unsigned int
bfdev_textsearch_find(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss)
{
    bfdev_ts_algorithm_t *algo = tsc->algo;
    tss->offset = 0;
    return algo->find(tsc, tss);
}

static inline unsigned int
bfdev_textsearch_next(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss)
{
    bfdev_ts_algorithm_t *algo = tsc->algo;
    return algo->find(tsc, tss);
}

static inline const void *
textsearch_pattern_get(bfdev_ts_context_t *tsc)
{
    bfdev_ts_algorithm_t *algo = tsc->algo;
    return algo->pattern_get(tsc);
}

static inline unsigned int
bfdev_textsearch_pattern_len(bfdev_ts_context_t *tsc)
{
    bfdev_ts_algorithm_t *algo = tsc->algo;
    return algo->pattern_len(tsc);
}

static inline void
bfdev_textsearch_destroy(bfdev_ts_context_t *tsc)
{
    bfdev_ts_algorithm_t *algo = tsc->algo;
    algo->destroy(tsc);
}

extern unsigned int
bfdev_textsearch_linear_find(bfdev_ts_context_t *tsc, bfdev_ts_linear_t *linear,
                             const void *data, unsigned int len);

extern unsigned int
bfdev_textsearch_linear_next(bfdev_ts_context_t *tsc, bfdev_ts_linear_t *linear);

extern bfdev_ts_context_t *
bfdev_textsearch_create(const bfdev_alloc_t *alloc, const char *name,
                        const void *pattern, size_t len, unsigned long flags);

extern int
bfdev_textsearch_register(bfdev_ts_algorithm_t *algo);

extern void
bfdev_textsearch_unregister(bfdev_ts_algorithm_t *algo);

BFDEV_END_DECLS

#endif /* _BFDEV_TEXTSEARCH_H_ */
