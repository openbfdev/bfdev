/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */
#ifndef _BFDEV_GALLOCATOR_H_
#define _BFDEV_GALLOCATOR_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

allocator_t *gallocator_create();

#ifdef __cplusplus
}
#endif

#endif  /* _BFDEV_GALLOCATOR_H_ */
