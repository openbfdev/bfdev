/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-refcount"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/refcount.h>
#include <export.h>

export void
bfdev_refcnt_report(bfdev_refcnt_t *ref, bfdev_refcnt_saturation_t type)
{
    switch (type) {
        case BFDEV_REFCNT_ADD_UAF:
            bfdev_log_err("(%p) add use-after-free\n", ref);
            break;

        case BFDEV_REFCNT_ADD_LEAK:
            bfdev_log_err("(%p) add leaking\n", ref);
            break;

        case BFDEV_REFCNT_ADD_OVF:
            bfdev_log_err("(%p) add overflow\n", ref);
            break;

        case BFDEV_REFCNT_SUB_UAF:
            bfdev_log_err("(%p) sub use-after-free\n", ref);
            break;

        case BFDEV_REFCNT_SUB_LEAK:
            bfdev_log_err("(%p) sub leaking\n", ref);
            break;

        case BFDEV_REFCNT_SUB_OVF:
            bfdev_log_err("(%p) sub overflow\n", ref);
            break;

        case BFDEV_REFCNT_ADDNZ_LEAK:
            bfdev_log_err("(%p) addnz leaking\n", ref);
            break;

        case BFDEV_REFCNT_ADDNZ_OVF:
            bfdev_log_err("(%p) addnz overflow\n", ref);
            break;

        default:
            bfdev_log_err("(%p) unknow error\n", ref);
            return;
    }
}
