/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RINGBUF_H_
#define _BFDEV_RINGBUF_H_

#include <bfdev/config.h>
#include <bfdev/macro.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

struct bfdev_ringbuf {
    const struct bfdev_alloc *alloc;
    unsigned long in;
    unsigned long out;
    unsigned long mask;
    unsigned long esize;
    void *data;
};

/**
 * BFDEV_GENERIC_RINGBUF - define a generic ringbuf structure.
 * @datatype: ringbuf data type.
 * @ptrtype: ringbuf pointer containing data.
 * @rsize: ringbuf record size.
 */
#define BFDEV_GENERIC_RINGBUF(datatype, ptrtype, rsize) \
    union {                                             \
        struct bfdev_ringbuf ringbuf;                   \
        datatype *data;                                 \
        const datatype *cdata;                          \
        ptrtype *ptr;                                   \
        const ptrtype *cptr;                            \
        char (*rectype)[rsize];                         \
    }

/**
 * BFDEV_BODY_RINGBUF - generate the body of normal ringbuf.
 * @type: ringbuf contains the type of data.
 * @ptype: ringbuf pointer containing data.
 * @size: ringbuf buffer size.
 * @rsize: ringbuf record size.
 */
#define BFDEV_BODY_RINGBUF(type, ptype, size, rsize) {  \
    BFDEV_GENERIC_RINGBUF(type, ptype, rsize);          \
    type buff[((size < 2) ||                            \
        (size & (size - 1))) ? -1 : size];              \
}

/**
 * BFDEV_BODY_RINGBUF_DYNAMIC - generate the body of dynamic ringbuf.
 * @type: ringbuf contains the type of data.
 * @ptype: ringbuf pointer containing data.
 * @rsize: ringbuf record size.
 */
#define BFDEV_BODY_RINGBUF_DYNAMIC(type, ptype, rsize) {    \
    BFDEV_GENERIC_RINGBUF(type, ptype, rsize);              \
    type buff[0];                                           \
}

/**
 * BFDEV_RINGBUF_INIT - initialize normal ringbuf in compound literals.
 * @name: the name of ringbuf to init.
 */
#define BFDEV_RINGBUF_INIT(name)                            \
(typeof(name)) {                                            \
    .ringbuf = {                                            \
        .in = 0, .out = 0,                                  \
        .esize = sizeof(*(name).buff),                      \
        .mask = ARRAY_SIZE((name).buff) - 1,                \
        .data = (name).buff,                                \
    },                                                      \
}

/**
 * BFDEV_RINGBUF_DYNAMIC_INIT - initialize dynamic ringbuf in compound literals.
 * @name: the name of ringbuf to init.
 */
#define BFDEV_RINGBUF_DYNAMIC_INIT(name)                    \
(typeof(name)) {{                                           \
    .ringbuf = {                                            \
        .in = 0, .out = 0, .mask = 0, .data = NULL,         \
        .esize = sizeof(*(name).buff),                      \
    },                                                      \
}}

/**
 * BFDEV_STRUCT_RINGBUF - generate a normal ringbuf structure.
 * @type: ringbuf contains the type of data.
 * @size: ringbuf buffer size.
 */
#define BFDEV_STRUCT_RINGBUF(type, size) \
    struct BFDEV_BODY_RINGBUF(type, type, size, 0)

/**
 * BFDEV_STRUCT_RINGBUF_RECORD - generate a record ringbuf structure.
 * @type: ringbuf contains the type of data.
 * @size: ringbuf buffer size.
 * @record: ringbuf record size.
 */
#define BFDEV_STRUCT_RINGBUF_RECORD(type, size, record) \
    struct BFDEV_BODY_RINGBUF(type, type, size, record)

/**
 * BFDEV_DECLARE_STRUCT_RINGBUF_DYNAMIC - generate a dynamic ringbuf structure.
 * @type: ringbuf contains the type of data.
 */
#define BFDEV_DECLARE_STRUCT_RINGBUF_DYNAMIC(type) \
    struct BFDEV_BODY_RINGBUF_DYNAMIC(type, type, 0)

/**
 * BFDEV_STRUCT_RINGBUF_DYNAMIC_RECORD - generate a dynamic record ringbuf structure.
 * @type: ringbuf contains the type of data.
 * @record: ringbuf record size.
 */
#define BFDEV_STRUCT_RINGBUF_DYNAMIC_RECORD(type, record) \
    struct BFDEV_BODY_RINGBUF_DYNAMIC(type, type, record)

/**
 * BFDEV_DECLARE_RINGBUF - declare a normal ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 * @size: ringbuf buffer size.
 */
#define BFDEV_DECLARE_RINGBUF(name, type, size) \
    BFDEV_STRUCT_RINGBUF(type, size) name

/**
 * BFDEV_DECLARE_RINGBUF_RECORD - declare a record ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 * @size: ringbuf buffer size.
 * @record: ringbuf record size.
 */
#define BFDEV_DECLARE_RINGBUF_RECORD(name, type, size, record) \
    BFDEV_STRUCT_RINGBUF_RECORD(type, size, record) name

/**
 * BFDEV_DECLARE_RINGBUF_DYNAMIC - declare a dynamic ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 */
#define BFDEV_DECLARE_RINGBUF_DYNAMIC(name, type) \
    BFDEV_DECLARE_STRUCT_RINGBUF_DYNAMIC(type) name

/**
 * BFDEV_DECLARE_RINGBUF_DYNAMIC_RECORD - declare a dynamic record ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 * @record: ringbuf record size.
 */
#define BFDEV_DECLARE_RINGBUF_DYNAMIC_RECORD(name, type, record) \
    BFDEV_STRUCT_RINGBUF_DYNAMIC_RECORD(type, record) name

/**
 * BFDEV_DEFINE_RINGBUF - define a normal ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 * @size: ringbuf buffer size.
 */
#define BFDEV_DEFINE_RINGBUF(name, type, size) \
    BFDEV_DECLARE_RINGBUF(name, type, size) = BFDEV_RINGBUF_INIT(name)

/**
 * BFDEV_DEFINE_RINGBUF_RECORD - define a record ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 * @size: ringbuf buffer size.
 * @record: ringbuf record size.
 */
#define BFDEV_DEFINE_RINGBUF_RECORD(name, type, size, record) \
    BFDEV_DECLARE_RINGBUF_RECORD(name, type, size, record) = BFDEV_RINGBUF_INIT(name)

/**
 * BFDEV_DEFINE_RINGBUF_DYNAMIC - define a dynamic ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 */
#define BFDEV_DEFINE_RINGBUF_DYNAMIC(name, type) \
    BFDEV_DECLARE_RINGBUF_DYNAMIC(name, type) = BFDEV_RINGBUF_DYNAMIC_INIT(name)

/**
 * BFDEV_DEFINE_RINGBUF_DYNAMIC_RECORD - declare define dynamic record ringbuf structure.
 * @name: name of ringbuf structure to declare.
 * @type: ringbuf contains the type of data.
 * @record: ringbuf record size.
 */
#define BFDEV_DEFINE_RINGBUF_DYNAMIC_RECORD(name, type, record) \
    BFDEV_DECLARE_RINGBUF_DYNAMIC_RECORD(name, type, record) = BFDEV_RINGBUF_DYNAMIC_INIT(name)

/**
 * bfdev_ringbuf_initialized - check if the ringbuf is initialized.
 * @ptr: pointer of the ringbuf to check.
 */
#define bfdev_ringbuf_initialized(ptr) ((ptr)->ringbuf.mask)

/**
 * bfdev_ringbuf_recsize - get the size of the record length fie.
 * @ptr: pointer of the ringbuf to get __recsi.
 */
#define bfdev_ringbuf_recsize(ptr) (sizeof(*(ptr)->rectyp))

/**
 * bfdev_ringbuf_size - get the size of the element managed by the ringbuf.
 * @ptr: pointer of the ringbuf to get size.
 */
#define bfdev_ringbuf_size(ptr) ((ptr)->ringbuf.mask + 1)

/**
 * bfdev_ringbuf_esize - get the size of the ringbuf in elements.
 * @ptr: pointer of the ringbuf to get size.
 */
#define bfdev_ringbuf_esize(ptr) ((ptr)->ringbuf.esize)

/**
 * bfdev_ringbuf_reset - reset ringbuf state.
 * @ptr: the ringbuf to reset.
 */
#define bfdev_ringbuf_reset(ptr) do {               \
    typeof((ptr) + 1) __tmp = (ptr);                \
    __tmp->ringbuf.out = __tmp->ringbuf.in = 0;     \
} while (0)

/**
 * bfdev_ringbuf_homing - homing unread valid data length.
 * @ptr: the ringbuf to homing.
 */
#define bfdev_ringbuf_homing(ptr) do {              \
    typeof((ptr) + 1) __tmp = (ptr);                \
    __tmp->ringbuf.out = __tmp->ringbuf.in;         \
} while (0)

/**
 * bfdev_ringbuf_len - get the valid data length in ringbuf.
 * @ptr: the ringbuf to get.
 */
#define bfdev_ringbuf_len(ptr) ({                   \
    typeof((ptr) + 1) __tlen = (ptr);               \
    __tlen->ringbuf.in - __tlen->ringbuf.out;       \
})

/**
 * bfdev_ringbuf_check_empty - check w ringbuf is empty.
 * @ptr: the ringbuf to check.
 */
#define bfdev_ringbuf_check_empty(ptr) ({           \
    typeof((ptr) + 1) __ttmp = (ptr);               \
    __ttmp->ringbuf.in == __ttmp->ringbuf.out;      \
})

/**
 * bfdev_ringbuf_check_full - check whether ringbuf is full.
 * @ptr: the ringbuf to check.
 */
#define bfdev_ringbuf_check_full(ptr) ({                \
    typeof((ptr) + 1) __ttmp = (ptr);                   \
    bfdev_ringbuf_len(__ttmp) > __ttmp->ringbuf.mask;   \
})

/**
 * bfdev_ringbuf_check_dynamic - check whether ringbuf is dynamic.
 * @ptr: the ringbuf to check.
 */
#define bfdev_ringbuf_check_dynamic(ptr) (              \
    !sizeof(ptr->buff)                                  \
)

/**
 * bfdev_ringbuf_alloc - dynamically allocate buffer to ringbuf.
 * @ptr: the ringbuf to allocate buffer.
 * @size: size of buffer.
 */
#define bfdev_ringbuf_alloc(ptr, alloc, size) ({        \
    typeof((ptr) + 1) __tmp = (ptr);                    \
    bfdev_ringbuf_check_dynamic(__tmp) ?                \
    bfdev_ringbuf_dynamic_alloc(&__tmp->ringbuf,        \
    alloc, sizeof(*__tmp->data), size) :                \
    -BFDEV_EINVAL;                                      \
})

/**
 * bfdev_ringbuf_free - dynamically free buffer to ringbuf.
 * @ptr: the ringbuf to free buffer.
 */
#define bfdev_ringbuf_free(ptr) ({                      \
    typeof((ptr) + 1) __tmp = (ptr);                    \
    bfdev_ringbuf_check_dynamic(__tmp) ?                \
    bfdev_ringbuf_dynamic_free(&__tmp->ringbuf) :       \
    -BFDEV_EINVAL;                                      \
})

/**
 * bfdev_ringbuf_peek - peek an object from ringbuf.
 * @struct: the ringbuf to peek object out.
 * @value: object to peek.
 */
#define bfdev_ringbuf_peek(pringbuf, value)  ({             \
    typeof((pringbuf) + 1) __tmp = (pringbuf);              \
    typeof(__tmp->ptr) __tvalue = (value);                  \
    struct bfdev_ringbuf *__ringbuf = &__tmp->ringbuf;      \
    unsigned long __recsize = sizeof(*__tmp->rectype);      \
    unsigned long __retval;                                 \
    if (__recsize) {                                        \
        __retval = bfdev_ringbuf_peek_record(__ringbuf,     \
            __tvalue, sizeof(*__tvalue), __recsize);        \
    } else {                                                \
        __retval = !bfdev_ringbuf_check_empty(__tmp);       \
        if (__retval) {                                     \
            *(typeof(__tmp->data)) __tvalue =               \
            (bfdev_ringbuf_check_dynamic(__tmp) ?           \
            ((typeof(__tmp->data)) __ringbuf->data) :       \
            (__tmp->buff))                                  \
            [__ringbuf->out & __tmp->ringbuf.mask];         \
        }                                                   \
    }                                                       \
    __retval;                                               \
})

/**
 * bfdev_ringbuf_get - get an object from ringbuf.
 * @struct: the ringbuf to get object out.
 * @value: object to get.
 */
#define bfdev_ringbuf_get(pringbuf, value)  ({              \
    typeof((pringbuf) + 1) __tmp = (pringbuf);              \
    typeof(__tmp->ptr) __tvalue = (value);                  \
    struct bfdev_ringbuf *__ringbuf = &__tmp->ringbuf;      \
    unsigned long __recsize = sizeof(*__tmp->rectype);      \
    unsigned long __retval;                                 \
    if (__recsize) {                                        \
        __retval = bfdev_ringbuf_out_record(__ringbuf,      \
            __tvalue, sizeof(*__tvalue), __recsize);        \
    } else {                                                \
        __retval = !bfdev_ringbuf_check_empty(__tmp);       \
        if (__retval) {                                     \
            *(typeof(__tmp->data)) __tvalue =               \
            (bfdev_ringbuf_check_dynamic(__tmp) ?           \
            ((typeof(__tmp->data)) __ringbuf->data) :       \
            (__tmp->buff))                                  \
            [__ringbuf->out & __tmp->ringbuf.mask];         \
            ++__ringbuf->out;                               \
        }                                                   \
    }                                                       \
    __retval;                                               \
})

/**
 * bfdev_ringbuf_put - put an object into ringbuf.
 * @pringbuf: the ringbuf to put object in.
 * @value: object to put.
 */
#define bfdev_ringbuf_put(pringbuf, value)  ({              \
    typeof((pringbuf) + 1) __tmp = (pringbuf);              \
    typeof(*__tmp->cdata) __tvalue = (value);               \
    struct bfdev_ringbuf *__ringbuf = &__tmp->ringbuf;      \
    unsigned long __recsize = sizeof(*__tmp->rectype);      \
    unsigned long __retval;                                 \
    if (__recsize) {                                        \
        __retval = bfdev_ringbuf_in_record(__ringbuf,       \
            &__tvalue, sizeof(__tvalue), __recsize);        \
    } else {                                                \
        __retval = bfdev_ringbuf_check_full(__tmp);         \
        if (__retval)                                       \
            ++__ringbuf->out;                               \
        (bfdev_ringbuf_check_dynamic(__tmp) ?               \
        ((typeof(__tmp->data)) __ringbuf->data) :           \
        (__tmp->buff))                                      \
        [__ringbuf->in & __tmp->ringbuf.mask] =             \
        *(typeof(__tmp->data)) &__tvalue;                   \
        ++__ringbuf->in;                                    \
    }                                                       \
    __retval;                                               \
})

/**
 * bfdev_ringbuf_out_peek - peek continuous data from ringbuf.
 * @pringbuf: the ringbuf to peek data out.
 * @buff: the buffer to peek data in.
 * @len: number of continuously peeked objects.
 */
#define bfdev_ringbuf_out_peek(pringbuf, buff, len) ({                  \
    typeof((pringbuf) + 1) __tmp = (pringbuf);                          \
    typeof(__tmp->ptr) __tbuff = (buff);                                \
    struct bfdev_ringbuf *__ringbuf = &__tmp->ringbuf;                  \
    unsigned long __tlen = (len);                                       \
    unsigned long __recsize = sizeof(*__tmp->rectype);                  \
    (__recsize) ?                                                       \
    bfdev_ringbuf_peek_record(__ringbuf, __tbuff, __tlen, __recsize) :  \
    bfdev_ringbuf_peek_flat(__ringbuf, __tbuff, __tlen);                \
})

/**
 * bfdev_ringbuf_out - copy continuous data from ringbuf.
 * @pringbuf: the ringbuf to copy data out.
 * @buff: the buffer to copy data in.
 * @len: number of continuously copied objects.
 */
#define bfdev_ringbuf_out(pringbuf, buff, len) ({                       \
    typeof((pringbuf) + 1) __tmp = (pringbuf);                          \
    typeof(__tmp->ptr) __tbuff = (buff);                                \
    struct bfdev_ringbuf *__ringbuf = &__tmp->ringbuf;                  \
    unsigned long __tlen = (len);                                       \
    unsigned long __recsize = sizeof(*__tmp->rectype);                  \
    (__recsize) ?                                                       \
    bfdev_ringbuf_out_record(__ringbuf, __tbuff, __tlen, __recsize) :   \
    bfdev_ringbuf_out_flat(__ringbuf, __tbuff, __tlen);                 \
})

/**
 * bfdev_ringbuf_in - copy continuous data into ringbuf.
 * @pringbuf: the ringbuf to copy data in.
 * @buff: the buffer to copy data out.
 * @len: number of continuously copied objects.
 */
#define bfdev_ringbuf_in(pringbuf, buff, len) ({                        \
    typeof((pringbuf) + 1) __tmp = (pringbuf);                          \
    typeof(__tmp->cptr) __tbuff = (buff);                               \
    struct bfdev_ringbuf *__ringbuf = &__tmp->ringbuf;                  \
    unsigned long __tlen = (len);                                       \
    unsigned long __recsize = sizeof(*__tmp->rectype);                  \
    (__recsize) ?                                                       \
    bfdev_ringbuf_in_record(__ringbuf, __tbuff, __tlen, __recsize) :    \
    bfdev_ringbuf_in_flat(__ringbuf, __tbuff, __tlen);                  \
})

extern unsigned long bfdev_ringbuf_peek_flat(struct bfdev_ringbuf *ringbuf, void *buff, unsigned long len);
extern unsigned long bfdev_ringbuf_out_flat(struct bfdev_ringbuf *ringbuf, void *buff, unsigned long len);
extern unsigned long bfdev_ringbuf_in_flat(struct bfdev_ringbuf *ringbuf, const void *buff, unsigned long len);
extern unsigned long bfdev_ringbuf_peek_record(struct bfdev_ringbuf *ringbuf, void *buff, unsigned long len, unsigned long record);
extern unsigned long bfdev_ringbuf_out_record(struct bfdev_ringbuf *ringbuf, void *buff, unsigned long len, unsigned long record);
extern unsigned long bfdev_ringbuf_in_record(struct bfdev_ringbuf *ringbuf, const void *buff, unsigned long len, unsigned long record);
extern int bfdev_ringbuf_dynamic_alloc(struct bfdev_ringbuf *ringbuf, const struct bfdev_alloc *alloc, size_t esize, size_t size);
extern void bfdev_ringbuf_dynamic_free(struct bfdev_ringbuf *ringbuf);

#endif /* _BFDEV_RINGBUF_H_ */
