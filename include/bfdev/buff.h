/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BUFF_H_
#define _BFDEV_BUFF_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/string.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_buff bfdev_buff_t;

struct bfdev_buff {
    bfdev_size_t len;
    void *data;
};

#define BFDEV_BUFF_STATIC(LEN, DATA) { \
    .len = (LEN), .data = (void *)(DATA), \
}

#define BFDEV_BUFF_INIT(len, data) \
    (bfdev_buff_t) BFDEV_BUFF_STATIC(len, data)

#define BFDEV_DEFINE_BUFF(name, len, data) \
    bfdev_buff_t name = BFDEV_BUFF_INIT(len, data)

#define BFDEV_DEFINE_STRING(name, str) \
    BFDEV_DEFINE_BUFF(name, sizeof(str) - 1, str)

static inline void
bfdev_buff_init(bfdev_buff_t *buff, bfdev_size_t len, void *data)
{
    *buff = BFDEV_BUFF_INIT(len, data);
}

static inline void
bfdev_string_init(bfdev_buff_t *buff, const char *str)
{
    *buff = BFDEV_BUFF_INIT(bfdev_strlen(str), str);
}

static inline void *
bfdev_buff_data(bfdev_buff_t *buff)
{
    return buff->data;
}

static inline bfdev_size_t
bfdev_buff_len(bfdev_buff_t *buff)
{
    return buff->len;
}

BFDEV_END_DECLS

#endif /* _BFDEV_BUFF_H_ */
