/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#ifndef _BFDEV_ALLOCATOR_H_
#define _BFDEV_ALLOCATOR_H_

#include "stddef.h"
#include "stdlib.h"

typedef struct allocator {
    void *(*malloc)(size_t size, void *pdata);
    void *(*realloc)(void *p, unsigned long size, void *pdata);
    void (*free)(void *p, void *pdata);
    void *pdata;
} allocator_t;

#endif  /* _BFDEV_ALLOCATOR_H_ */
