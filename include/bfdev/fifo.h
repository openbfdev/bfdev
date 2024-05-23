/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_FIFO_H_
#define _BFDEV_FIFO_H_

#include <bfdev/config.h>
#include <bfdev/macro.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_fifo bfdev_fifo_t;

struct bfdev_fifo {
    const bfdev_alloc_t *alloc;
    unsigned long in;
    unsigned long out;
    unsigned long mask;
    unsigned long esize;
    void *data;
};

/**
 * BFDEV_GENERIC_FIFO() - define a generic fifo structure.
 * @datatype: fifo data type.
 * @ptrtype: fifo pointer containing data.
 * @rsize: fifo record size.
 */
#define BFDEV_GENERIC_FIFO(datatype, ptrtype, rsize)    \
    union {                                             \
        bfdev_fifo_t fifo;                              \
        datatype *data;                                 \
        const datatype *cdata;                          \
        ptrtype *ptr;                                   \
        const ptrtype *cptr;                            \
        char (*rectype)[rsize];                         \
    }

/**
 * BFDEV_BODY_FIFO() - generate the body of normal fifo.
 * @type: fifo contains the type of data.
 * @ptype: fifo pointer containing data.
 * @size: fifo buffer size.
 * @rsize: fifo record size.
 */
#define BFDEV_BODY_FIFO(type, ptype, size, rsize) {     \
    BFDEV_GENERIC_FIFO(type, ptype, rsize);             \
    type buff[((size < 2) ||                            \
        (size & (size - 1))) ? -1 : size];              \
}

/**
 * BFDEV_BODY_FIFO_DYNAMIC() - generate the body of dynamic fifo.
 * @type: fifo contains the type of data.
 * @ptype: fifo pointer containing data.
 * @rsize: fifo record size.
 */
#define BFDEV_BODY_FIFO_DYNAMIC(type, ptype, rsize) {   \
    BFDEV_GENERIC_FIFO(type, ptype, rsize);             \
    type buff[0];                                       \
}

/**
 * BFDEV_FIFO_INIT() - initialize normal fifo in compound literals.
 * @ptr: the pointer of fifo to init.
 */
#define BFDEV_FIFO_INIT(ptr)                            \
(typeof(*(ptr))) {                                      \
    .fifo = {                                           \
        .in = 0, .out = 0,                              \
        .esize = sizeof(*(ptr)->buff),                  \
        .mask = BFDEV_ARRAY_SIZE((ptr)->buff) - 1,      \
        .data = &(ptr)->buff,                           \
    },                                                  \
}

/**
 * BFDEV_FIFO_DYNAMIC_INIT() - initialize dynamic fifo in compound literals.
 * @ptr: the pointer of fifo to init.
 */
#define BFDEV_FIFO_DYNAMIC_INIT(ptr)                    \
(typeof(*(ptr))) {                                      \
    .fifo = {                                           \
        .in = 0, .out = 0, .mask = 0, .data = NULL,     \
        .esize = sizeof(*(ptr)->buff),                  \
    },                                                  \
}

/**
 * BFDEV_STRUCT_FIFO() - generate a normal fifo structure.
 * @type: fifo contains the type of data.
 * @size: fifo buffer size.
 */
#define BFDEV_STRUCT_FIFO(type, size) \
    struct BFDEV_BODY_FIFO(type, type, size, 0)

/**
 * BFDEV_STRUCT_FIFO_RECORD() - generate a record fifo structure.
 * @type: fifo contains the type of data.
 * @size: fifo buffer size.
 * @record: fifo record size.
 */
#define BFDEV_STRUCT_FIFO_RECORD(type, size, record) \
    struct BFDEV_BODY_FIFO(type, type, size, record)

/**
 * BFDEV_DECLARE_STRUCT_FIFO_DYNAMIC() - generate a dynamic fifo structure.
 * @type: fifo contains the type of data.
 */
#define BFDEV_DECLARE_STRUCT_FIFO_DYNAMIC(type) \
    struct BFDEV_BODY_FIFO_DYNAMIC(type, type, 0)

/**
 * BFDEV_STRUCT_FIFO_DYNAMIC_RECORD() - generate a dynamic record fifo structure.
 * @type: fifo contains the type of data.
 * @record: fifo record size.
 */
#define BFDEV_STRUCT_FIFO_DYNAMIC_RECORD(type, record) \
    struct BFDEV_BODY_FIFO_DYNAMIC(type, type, record)

/**
 * BFDEV_DECLARE_FIFO() - declare a normal fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 * @size: fifo buffer size.
 */
#define BFDEV_DECLARE_FIFO(name, type, size) \
    BFDEV_STRUCT_FIFO(type, size) name

/**
 * BFDEV_DECLARE_FIFO_RECORD() - declare a record fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 * @size: fifo buffer size.
 * @record: fifo record size.
 */
#define BFDEV_DECLARE_FIFO_RECORD(name, type, size, record) \
    BFDEV_STRUCT_FIFO_RECORD(type, size, record) name

/**
 * BFDEV_DECLARE_FIFO_DYNAMIC() - declare a dynamic fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 */
#define BFDEV_DECLARE_FIFO_DYNAMIC(name, type) \
    BFDEV_DECLARE_STRUCT_FIFO_DYNAMIC(type) name

/**
 * BFDEV_DECLARE_FIFO_DYNAMIC_RECORD() - declare a dynamic record fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 * @record: fifo record size.
 */
#define BFDEV_DECLARE_FIFO_DYNAMIC_RECORD(name, type, record) \
    BFDEV_STRUCT_FIFO_DYNAMIC_RECORD(type, record) name

/**
 * BFDEV_DEFINE_FIFO() - define a normal fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 * @size: fifo buffer size.
 */
#define BFDEV_DEFINE_FIFO(name, type, size) \
    BFDEV_DECLARE_FIFO(name, type, size) = BFDEV_FIFO_INIT(&name)

/**
 * BFDEV_DEFINE_FIFO_RECORD() - define a record fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 * @size: fifo buffer size.
 * @record: fifo record size.
 */
#define BFDEV_DEFINE_FIFO_RECORD(name, type, size, record) \
    BFDEV_DECLARE_FIFO_RECORD(name, type, size, record) = BFDEV_FIFO_INIT(&name)

/**
 * BFDEV_DEFINE_FIFO_DYNAMIC() - define a dynamic fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 */
#define BFDEV_DEFINE_FIFO_DYNAMIC(name, type) \
    BFDEV_DECLARE_FIFO_DYNAMIC(name, type) = BFDEV_FIFO_DYNAMIC_INIT(&name)

/**
 * BFDEV_DEFINE_FIFO_DYNAMIC_RECORD() - declare define dynamic record fifo structure.
 * @name: name of fifo structure to declare.
 * @type: fifo contains the type of data.
 * @record: fifo record size.
 */
#define BFDEV_DEFINE_FIFO_DYNAMIC_RECORD(name, type, record) \
    BFDEV_DECLARE_FIFO_DYNAMIC_RECORD(name, type, record) = BFDEV_FIFO_DYNAMIC_INIT(&name)

/**
 * bfdev_fifo_initialized() - check if the fifo is initialized.
 * @ptr: pointer of the fifo to check.
 */
#define bfdev_fifo_initialized(ptr) ((ptr)->fifo.mask)

/**
 * bfdev_fifo_recsize() - get the size of the record length field.
 * @ptr: pointer of the fifo to get field length.
 */
#define bfdev_fifo_recsize(ptr) (sizeof(*(ptr)->rectype))

/**
 * bfdev_fifo_size() - get the size of the element managed by the fifo.
 * @ptr: pointer of the fifo to get size.
 */
#define bfdev_fifo_size(ptr) ((ptr)->fifo.mask + 1)

/**
 * bfdev_fifo_esize() - get the size of the fifo in elements.
 * @ptr: pointer of the fifo to get size.
 */
#define bfdev_fifo_esize(ptr) ((ptr)->fifo.esize)

/**
 * bfdev_fifo_reset() - reset fifo state.
 * @ptr: the fifo to reset.
 */
#define bfdev_fifo_reset(ptr) do {              \
    typeof((ptr) + 1) __tmp = (ptr);            \
    __tmp->fifo.out = __tmp->fifo.in = 0;       \
} while (0)

/**
 * bfdev_fifo_homing() - homing unread valid data length.
 * @ptr: the fifo to homing.
 */
#define bfdev_fifo_homing(ptr) do {             \
    typeof((ptr) + 1) __tmp = (ptr);            \
    __tmp->fifo.out = __tmp->fifo.in;           \
} while (0)

/**
 * bfdev_fifo_len() - get the valid data length in fifo.
 * @ptr: the fifo to get.
 */
#define bfdev_fifo_len(ptr) ({                  \
    typeof((ptr) + 1) __tlen = (ptr);           \
    __tlen->fifo.in - __tlen->fifo.out;         \
})

/**
 * bfdev_fifo_check_empty() - check w fifo is empty.
 * @ptr: the fifo to check.
 */
#define bfdev_fifo_check_empty(ptr) ({          \
    typeof((ptr) + 1) __ttmp = (ptr);           \
    __ttmp->fifo.in == __ttmp->fifo.out;        \
})

/**
 * bfdev_fifo_check_full() - check whether fifo is full.
 * @ptr: the fifo to check.
 */
#define bfdev_fifo_check_full(ptr) ({           \
    typeof((ptr) + 1) __ttmp = (ptr);           \
    bfdev_fifo_len(__ttmp) > __ttmp->fifo.mask; \
})

/**
 * bfdev_fifo_check_dynamic() - check whether fifo is dynamic.
 * @ptr: the fifo to check.
 */
#define bfdev_fifo_check_dynamic(ptr) (         \
    !sizeof(ptr->buff)                          \
)

/**
 * bfdev_fifo_alloc() - dynamically allocate buffer to fifo.
 * @ptr: the fifo to allocate buffer.
 * @size: size of buffer.
 */
#define bfdev_fifo_alloc(ptr, alloc, size) ({   \
    typeof((ptr) + 1) __tmp = (ptr);            \
    bfdev_fifo_check_dynamic(__tmp) ?           \
    bfdev_fifo_dynamic_alloc(&__tmp->fifo,      \
    alloc, sizeof(*__tmp->data), size) :        \
    -BFDEV_EINVAL;                              \
})

/**
 * bfdev_fifo_free() - dynamically free buffer to fifo.
 * @ptr: the fifo to free buffer.
 */
#define bfdev_fifo_free(ptr) ({                 \
    typeof((ptr) + 1) __tmp = (ptr);            \
    bfdev_fifo_check_dynamic(__tmp) ?           \
    bfdev_fifo_dynamic_free(&__tmp->fifo) :     \
    -BFDEV_EINVAL;                              \
})

/**
 * bfdev_fifo_peek() - peek an object from fifo.
 * @struct: the fifo to peek object out.
 * @value: object to peek.
 */
#define bfdev_fifo_peek(pfifo, value)  ({                   \
    typeof((pfifo) + 1) __tmp = (pfifo);                    \
    typeof(__tmp->ptr) __tvalue = (value);                  \
    bfdev_fifo_t *__fifo = &__tmp->fifo;                    \
    unsigned long __recsize = sizeof(*__tmp->rectype);      \
    unsigned long __retval;                                 \
    if (__recsize) {                                        \
        __retval = bfdev_fifo_peek_record(__fifo,           \
            __tvalue, sizeof(*__tvalue), __recsize);        \
    } else {                                                \
        __retval = !bfdev_fifo_check_empty(__tmp);          \
        if (__retval) {                                     \
            *(typeof(__tmp->data)) __tvalue =               \
            (bfdev_fifo_check_dynamic(__tmp) ?              \
            ((typeof(__tmp->data)) __fifo->data) :          \
            (__tmp->buff))                                  \
            [__fifo->out & __tmp->fifo.mask];               \
        }                                                   \
    }                                                       \
    __retval;                                               \
})

/**
 * bfdev_fifo_get() - get an object from fifo.
 * @struct: the fifo to get object out.
 * @value: object to get.
 */
#define bfdev_fifo_get(pfifo, value)  ({                    \
    typeof((pfifo) + 1) __tmp = (pfifo);                    \
    typeof(__tmp->ptr) __tvalue = (value);                  \
    bfdev_fifo_t *__fifo = &__tmp->fifo;                    \
    unsigned long __recsize = sizeof(*__tmp->rectype);      \
    unsigned long __retval;                                 \
    if (__recsize) {                                        \
        __retval = bfdev_fifo_out_record(__fifo,            \
            __tvalue, sizeof(*__tvalue), __recsize);        \
    } else {                                                \
        __retval = !bfdev_fifo_check_empty(__tmp);          \
        if (__retval) {                                     \
            *(typeof(__tmp->data)) __tvalue =               \
            (bfdev_fifo_check_dynamic(__tmp) ?              \
            ((typeof(__tmp->data)) __fifo->data) :          \
            (__tmp->buff))                                  \
            [__fifo->out & __tmp->fifo.mask];               \
            ++__fifo->out;                                  \
        }                                                   \
    }                                                       \
    __retval;                                               \
})

/**
 * bfdev_fifo_put() - put an object into fifo.
 * @pfifo: the fifo to put object in.
 * @value: object to put.
 */
#define bfdev_fifo_put(pfifo, value)  ({                    \
    typeof((pfifo) + 1) __tmp = (pfifo);                    \
    typeof(*__tmp->cdata) __tvalue = (value);               \
    bfdev_fifo_t *__fifo = &__tmp->fifo;                    \
    unsigned long __recsize = sizeof(*__tmp->rectype);      \
    unsigned long __retval;                                 \
    if (__recsize) {                                        \
        __retval = bfdev_fifo_in_record(__fifo,             \
            &__tvalue, sizeof(__tvalue), __recsize);        \
    } else {                                                \
        __retval = !bfdev_fifo_check_full(__tmp);           \
        if (__retval) {                                     \
            (bfdev_fifo_check_dynamic(__tmp) ?              \
            ((typeof(__tmp->data)) __fifo->data) :          \
            (__tmp->buff))                                  \
            [__fifo->in & __tmp->fifo.mask] =               \
            *(typeof(__tmp->data)) &__tvalue;               \
            ++__fifo->in;                                   \
        }                                                   \
    }                                                       \
    __retval;                                               \
})

/**
 * bfdev_fifo_out_peek() - peek continuous data from fifo.
 * @pfifo: the fifo to peek data out.
 * @buff: the buffer to peek data in.
 * @len: number of continuously peeked objects.
 */
#define bfdev_fifo_out_peek(pfifo, buff, len) ({                    \
    typeof((pfifo) + 1) __tmp = (pfifo);                            \
    typeof(__tmp->ptr) __tbuff = (buff);                            \
    bfdev_fifo_t *__fifo = &__tmp->fifo;                            \
    unsigned long __tlen = (len);                                   \
    unsigned long __recsize = sizeof(*__tmp->rectype);              \
    (__recsize) ?                                                   \
    bfdev_fifo_peek_record(__fifo, __tbuff, __tlen, __recsize) :    \
    bfdev_fifo_peek_flat(__fifo, __tbuff, __tlen);                  \
})

/**
 * bfdev_fifo_out() - copy continuous data from fifo.
 * @pfifo: the fifo to copy data out.
 * @buff: the buffer to copy data in.
 * @len: number of continuously copied objects.
 */
#define bfdev_fifo_out(pfifo, buff, len) ({                         \
    typeof((pfifo) + 1) __tmp = (pfifo);                            \
    typeof(__tmp->ptr) __tbuff = (buff);                            \
    bfdev_fifo_t *__fifo = &__tmp->fifo;                            \
    unsigned long __tlen = (len);                                   \
    unsigned long __recsize = sizeof(*__tmp->rectype);              \
    (__recsize) ?                                                   \
    bfdev_fifo_out_record(__fifo, __tbuff, __tlen, __recsize) :     \
    bfdev_fifo_out_flat(__fifo, __tbuff, __tlen);                   \
})

/**
 * bfdev_fifo_in() - copy continuous data into fifo.
 * @pfifo: the fifo to copy data in.
 * @buff: the buffer to copy data out.
 * @len: number of continuously copied objects.
 */
#define bfdev_fifo_in(pfifo, buff, len) ({                          \
    typeof((pfifo) + 1) __tmp = (pfifo);                            \
    typeof(__tmp->cptr) __tbuff = (buff);                           \
    bfdev_fifo_t *__fifo = &__tmp->fifo;                            \
    unsigned long __tlen = (len);                                   \
    unsigned long __recsize = sizeof(*__tmp->rectype);              \
    (__recsize) ?                                                   \
    bfdev_fifo_in_record(__fifo, __tbuff, __tlen, __recsize) :      \
    bfdev_fifo_in_flat(__fifo, __tbuff, __tlen);                    \
})

extern unsigned long
bfdev_fifo_peek_flat(bfdev_fifo_t *fifo, void *buff, unsigned long len);

extern unsigned long
bfdev_fifo_out_flat(bfdev_fifo_t *fifo, void *buff, unsigned long len);

extern unsigned long
bfdev_fifo_in_flat(bfdev_fifo_t *fifo, const void *buff, unsigned long len);

extern unsigned long
bfdev_fifo_peek_record(bfdev_fifo_t *fifo, void *buff, unsigned long len,
                       unsigned long record);

extern unsigned long
bfdev_fifo_out_record(bfdev_fifo_t *fifo, void *buff, unsigned long len,
                      unsigned long record);

extern unsigned long
bfdev_fifo_in_record(bfdev_fifo_t *fifo, const void *buff, unsigned long len,
                     unsigned long record);

extern int
bfdev_fifo_dynamic_alloc(bfdev_fifo_t *fifo, const bfdev_alloc_t *alloc,
                         size_t esize, size_t size);

extern void
bfdev_fifo_dynamic_free(bfdev_fifo_t *fifo);

BFDEV_END_DECLS

#endif  /* _BFDEV_FIFO_H_ */
