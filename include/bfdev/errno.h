/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ERRNO_H_
#define _BFDEV_ERRNO_H_

#define BFDEV_ENOERR                0       /* No error */
#define BFDEV_EPERM                 1       /* Operation not permitted */
#define BFDEV_ENOENT                2       /* No such file or directory */
#define BFDEV_ESRCH                 3       /* No such process */
#define BFDEV_EINTR                 4       /* Interrupted system call */
#define BFDEV_EIO                   5       /* I/O error */
#define BFDEV_ENXIO                 6       /* No such device or address */
#define BFDEV_E2BIG                 7       /* Argument list too long */
#define BFDEV_ENOEXEC               8       /* Exec format error */
#define BFDEV_EBADF                 9       /* Bad file number */
#define BFDEV_ECHILD                10      /* No child processes */
#define BFDEV_EAGAIN                11      /* Try again */
#define BFDEV_ENOMEM                12      /* Out of memory */
#define BFDEV_EACCES                13      /* Permission denied */
#define BFDEV_EFAULT                14      /* Bad address */
#define BFDEV_ENOTBLK               15      /* Block device required */
#define BFDEV_EBUSY                 16      /* Device or resource busy */
#define BFDEV_EEXIST                17      /* File exists */
#define BFDEV_EXDEV                 18      /* Cross-device link */
#define BFDEV_ENODEV                19      /* No such device */
#define BFDEV_ENOTDIR               20      /* Not a directory */
#define BFDEV_EISDIR                21      /* Is a directory */
#define BFDEV_EINVAL                22      /* Invalid argument */
#define BFDEV_ENFILE                23      /* File table overflow */
#define BFDEV_EMFILE                24      /* Too many open files */
#define BFDEV_ENOTTY                25      /* Not a typewriter */
#define BFDEV_ETXTBSY               26      /* Text file busy */
#define BFDEV_EFBIG                 27      /* File too large */
#define BFDEV_ENOSPC                28      /* No space left on device */
#define BFDEV_ESPIPE                29      /* Illegal seek */
#define BFDEV_EROFS                 30      /* Read-only file system */
#define BFDEV_EMLINK                31      /* Too many links */
#define BFDEV_EPIPE                 32      /* Broken pipe */
#define BFDEV_EDOM                  33      /* Math argument out of domain of func */
#define BFDEV_ERANGE                34      /* Math result not representable */
#define BFDEV_EDEADLK               35      /* Resource deadlock would occur */
#define BFDEV_ENAMETOOLONG          36      /* File name too long */
#define BFDEV_ENOLCK                37      /* No record locks available */
#define BFDEV_ENOSYS                38      /* Invalid system call number */
#define BFDEV_EBFONT                59      /* Bad font file format */
#define BFDEV_ENOSTR                60      /* Device not a stream */
#define BFDEV_ENODATA               61      /* No data available */
#define BFDEV_ETIME                 62      /* Timer expired */
#define BFDEV_ENOSR                 63      /* Out of streams resources */
#define BFDEV_ENONET                64      /* Machine is not on the network */
#define BFDEV_ENOPKG                65      /* Package not installed */
#define BFDEV_EREMOTE               66      /* Object is remote */
#define BFDEV_ENOLINK               67      /* Link has been severed */
#define BFDEV_EADV                  68      /* Advertise error */
#define BFDEV_ESRMNT                69      /* Srmount error */
#define BFDEV_ECOMM                 70      /* Communication error on send */
#define BFDEV_EPROTO                71      /* Protocol error */
#define BFDEV_EMULTIHOP             72      /* Multihop attempted */
#define BFDEV_EDOTDOT               73      /* RFS specific error */
#define BFDEV_EBADMSG               74      /* Not a data message */
#define BFDEV_EOVERFLOW             75      /* Value too large for defined data type */
#define BFDEV_ENOTUNIQ              76      /* Name not unique on network */
#define BFDEV_EBADFD                77      /* File descriptor in bad state */
#define BFDEV_EREMCHG               78      /* Remote address changed */
#define BFDEV_ELIBACC               79      /* Can not access a needed shared library */
#define BFDEV_ELIBBAD               80      /* Accessing a corrupted shared library */
#define BFDEV_ELIBSCN               81      /* .lib section in a.out corrupted */
#define BFDEV_ELIBMAX               82      /* Attempting to link in too many shared libraries */
#define BFDEV_ELIBEXEC              83      /* Cannot exec a shared library directly */
#define BFDEV_EILSEQ                84      /* Illegal byte sequence */
#define BFDEV_ERESTART              85      /* Interrupted system call should be restarted */
#define BFDEV_ESTRPIPE              86      /* Streams pipe error */
#define BFDEV_EUSERS                87      /* Too many users */
#define BFDEV_ENOTSOCK              88      /* Socket operation on non-socket */
#define BFDEV_EDESTADDRREQ          89      /* Destination address required */
#define BFDEV_EMSGSIZE              90      /* Message too long */
#define BFDEV_EPROTOTYPE            91      /* Protocol wrong type for socket */
#define BFDEV_ENOPROTOOPT           92      /* Protocol not available */
#define BFDEV_EPROTONOSUPPORT       93      /* Protocol not supported */
#define BFDEV_ESOCKTNOSUPPORT       94      /* Socket type not supported */
#define BFDEV_EOPNOTSUPP            95      /* Operation not supported on transport endpoint */
#define BFDEV_EPFNOSUPPORT          96      /* Protocol family not supported */
#define BFDEV_EAFNOSUPPORT          97      /* Address family not supported by protocol */
#define BFDEV_EADDRINUSE            98      /* Address already in use */
#define BFDEV_EADDRNOTAVAIL         99      /* Cannot assign requested address */
#define BFDEV_ENETDOWN              100     /* Network is down */
#define BFDEV_ENETUNREACH           101     /* Network is unreachable */
#define BFDEV_ENETRESET             102     /* Network dropped connection because of reset */
#define BFDEV_ECONNABORTED          103     /* Software caused connection abort */
#define BFDEV_ECONNRESET            104     /* Connection reset by peer */
#define BFDEV_ENOBUFS               105     /* No buffer space available */
#define BFDEV_EISCONN               106     /* Transport endpoint is already connected */
#define BFDEV_ENOTCONN              107     /* Transport endpoint is not connected */
#define BFDEV_ESHUTDOWN             108     /* Cannot send after transport endpoint shutdown */
#define BFDEV_ETOOMANYREFS          109     /* Too many references: cannot splice */
#define BFDEV_ETIMEDOUT             110     /* Connection timed out */
#define BFDEV_ECONNREFUSED          111     /* Connection refused */
#define BFDEV_EHOSTDOWN             112     /* Host is down */
#define BFDEV_EHOSTUNREACH          113     /* No route to host */
#define BFDEV_EALREADY              114     /* Operation already in progress */
#define BFDEV_EINPROGRESS           115     /* Operation now in progress */
#define BFDEV_ESTALE                116     /* Stale file handle */
#define BFDEV_EUCLEAN               117     /* Structure needs cleaning */
#define BFDEV_ENOTNAM               118     /* Not a XENIX named type file */
#define BFDEV_ENAVAIL               119     /* No XENIX semaphores available */
#define BFDEV_EISNAM                120     /* Is a named type file */
#define BFDEV_EREMOTEIO             121     /* Remote I/O error */
#define BFDEV_EDQUOT                122     /* Quota exceeded */
#define BFDEV_ENOMEDIUM             123     /* No medium found */
#define BFDEV_EMEDIUMTYPE           124     /* Wrong medium type */
#define BFDEV_ECANCELED             125     /* Operation Canceled */
#define BFDEV_ENOKEY                126     /* Required key not available */
#define BFDEV_EKEYEXPIRED           127     /* Key has expired */
#define BFDEV_EKEYREVOKED           128     /* Key has been revoked */
#define BFDEV_EKEYREJECTED          129     /* Key was rejected by service */
#define BFDEV_ERRNO_MAX             4096

#endif /* _BFDEV_ERRNO_H_ */
