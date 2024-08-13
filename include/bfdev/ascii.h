/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASCII_H_
#define _BFDEV_ASCII_H_

#define BFDEV_ASCII_NUL     0x00    /* Null Char */
#define BFDEV_ASCII_SOH     0x01    /* Start Of Heading */
#define BFDEV_ASCII_STX     0x02    /* Start Of Text */
#define BFDEV_ASCII_ETX     0x03    /* End Of Text */
#define BFDEV_ASCII_EOT     0x04    /* End Of Transmission */
#define BFDEV_ASCII_ENQ     0x05    /* Enquiry */
#define BFDEV_ASCII_ACK     0x06    /* Acknowledgment */
#define BFDEV_ASCII_BEL     0x07    /* Bell */
#define BFDEV_ASCII_BS      0x08    /* Back Space */
#define BFDEV_ASCII_HT      0x09    /* Horizontal Tab */
#define BFDEV_ASCII_LF      0x0a    /* Line Feed */
#define BFDEV_ASCII_VT      0x0b    /* Vertical Tab */
#define BFDEV_ASCII_FF      0x0c    /* Form Feed */
#define BFDEV_ASCII_CR      0x0d    /* Carriage Return */
#define BFDEV_ASCII_SO      0x0e    /* Shift Out / X-On */
#define BFDEV_ASCII_SI      0x0f    /* Shift In / X-Off */
#define BFDEV_ASCII_DLE     0x10    /* Data Line Escape */
#define BFDEV_ASCII_DC1     0x11    /* Device Control 1 (Oft. Xon) */
#define BFDEV_ASCII_DC2     0x12    /* Device Control 2 */
#define BFDEV_ASCII_DC3     0x13    /* Device Control 3 (Oft. Xoff) */
#define BFDEV_ASCII_DC4     0x14    /* Device Control 4 */
#define BFDEV_ASCII_NAK     0x15    /* Negative Acknowledgement */
#define BFDEV_ASCII_SYN     0x16    /* Synchronous Idle */
#define BFDEV_ASCII_ETB     0x17    /* End Of Transmit Block */
#define BFDEV_ASCII_CAN     0x18    /* Cancel */
#define BFDEV_ASCII_EM      0x19    /* End Of Medium */
#define BFDEV_ASCII_SUB     0x1a    /* Substitute */
#define BFDEV_ASCII_ESC     0x1b    /* Escape */
#define BFDEV_ASCII_FS      0x1c    /* File Separator */
#define BFDEV_ASCII_GS      0x1d    /* Group Separator */
#define BFDEV_ASCII_RS      0x1e    /* Record Separator */
#define BFDEV_ASCII_US      0x1f    /* Unit Separator */
#define BFDEV_ASCII_DEL     0x7f    /* Delete */

#endif /* _BFDEV_ASCII_H_ */
