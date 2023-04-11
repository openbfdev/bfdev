/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#ifndef _BFDEV_ATOMIC_H_
#define _BFDEV_ATOMIC_H_

#include <bfdev/stdint.h>
#include <bfdev/config.h>

#ifdef BFDEV_HAVE_GCC_ATOMIC

#define bfdev_atomic_cmp_set(lock, old, set)                                    \
    __sync_bool_compare_and_swap(lock, old, set)


#define bfdev_atomic_xchg(lock, set)                                            \
    __sync_lock_test_and_set(lock, set)


#define bfdev_atomic_fetch_add(addr, value)                                     \
    __sync_fetch_and_add(addr, value)


#define bfdev_atomic_fetch_sub(addr, value)                                     \
    __sync_fetch_and_sub(addr, value)


#define bfdev_atomic_fetch_or(addr, value)                                      \
    __sync_fetch_and_or(addr, value)


#define bfdev_atomic_fetch_and(addr, value)                                     \
    __sync_fetch_and_and(addr, value)


#define bfdev_atomic_fetch_xor(addr, value)                                     \
    __sync_fetch_and_xor(addr, value)


#define bfdev_atomic_fetch_nand(addr, value)                                    \
    __sync_fetch_and_nand(addr, value)


#define bfdev_atomic_add_fetch(addr, value)                                     \
    __sync_add_and_fetch(addr, value)


#define bfdev_atomic_sub_fetch(addr, value)                                     \
    __sync_sub_and_fetch(addr, value)


#define bfdev_atomic_or_fetch(addr, value)                                      \
    __sync_or_and_fetch(addr, value)


#define bfdev_atomic_and_fetch(addr, value)                                     \
    __sync_and_and_fetch(addr, value)


#define bfdev_atomic_xor_fetch(addr, value)                                     \
    __sync_xor_and_fetch(addr, value)


#define bfdev_atomic_nand_fetch(addr, value)                                    \
    __sync_nand_and_fetch(addr, value)

#else

#error Your Compiler Dont Support Atomic, We May Process it in Feature

#endif

#endif /* _BFDEV_ATOMIC_H_ */
