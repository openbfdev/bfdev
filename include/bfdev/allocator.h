/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#ifndef _BFDEV_ALLOCATOR_H_
#define _BFDEV_ALLOCATOR_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct allocator {
    void *(*malloc)(size_t size, void *pdata);
    void *(*realloc)(void *p, unsigned long size, void *pdata);
    void (*free)(void *p, void *pdata);
    void *pdata;
} allocator_t;

#ifdef __cplusplus
}
#endif

#endif  /* _BFDEV_ALLOCATOR_H_ */
