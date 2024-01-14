/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CIRCLE_H_
#define _BFDEV_CIRCLE_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_circle bfdev_circle_t;

struct bfdev_circle {
    void *buffer;
    unsigned long head;
    unsigned long tail;
};

/* Return count in buffer */
#define BFDEV_CIRCLE_CNT(head, tail, size) \
    (((head) - (tail)) & ((size) - 1))

/* Return space available */
#define BFDEV_CIRCLE_SPACE(head, tail, size) \
    (((tail) - ((head) + 1)) & ((size) - 1))

/* Return count up to the end of the buffer */
#define BFDEV_CIRCLE_CNT_END(head, tail, size) ({   \
    unsigned long __end, __len;                     \
    __end = (size) - (tail);                        \
    __len = ((head) + __end) & ((size) - 1);        \
    __len < __end ? __len : __end;                  \
})

/* Return space available up to the end of the buffer */
#define BFDEV_CIRCLE_SPACE_END(head, tail, size) ({ \
    unsigned long __end, __len;                     \
    __end = ((size) - 1) - (head);                  \
    __len = (__end + (tail)) & ((size) - 1);        \
    __len <= end ? __len : end + 1;                 \
})

BFDEV_END_DECLS

#endif /* _BFDEV_CIRCLE_H_ */
