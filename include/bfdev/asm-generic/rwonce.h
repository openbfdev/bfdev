/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_RWONCE_H_
#define _BFDEV_ASM_GENERIC_RWONCE_H_

#define BFDEV_READ_ONCE(x) ({ \
    *(volatile typeof(x) *)&(x); \
})

#define BFDEV_WRITE_ONCE(x, val) ({ \
    *(volatile typeof(x) *)&(x) = (val); \
})

#endif /* _BFDEV_ASM_GENERIC_RWONCE_H_ */
