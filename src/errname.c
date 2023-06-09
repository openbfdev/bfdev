/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/bsearch.h>
#include <bfdev/errname.h>
#include <export.h>

#define ERRNAME(errnum, infos) { \
    BFDEV_##errnum, #errnum, infos \
}

export const struct bfdev_errname
bfdev_errname_table[] = {
    ERRNAME(ENOERR, "No error"),
    ERRNAME(EPERM, "Operation not permitted"),
    ERRNAME(ENOENT, "No such file or directory"),
    ERRNAME(ESRCH, "No such process"),
    ERRNAME(EINTR, "Interrupted system call"),
    ERRNAME(EIO, "I/O error"),
    ERRNAME(ENXIO, "No such device or address"),
    ERRNAME(E2BIG, "Argument list too long"),
    ERRNAME(ENOEXEC, "Exec format error"),
    ERRNAME(EBADF, "Bad file number"),
    ERRNAME(ECHILD, "No child processes"),
    ERRNAME(EAGAIN, "Try again"),
    ERRNAME(ENOMEM, "Out of memory"),
    ERRNAME(EACCES, "Permission denied"),
    ERRNAME(EFAULT, "Bad address"),
    ERRNAME(ENOTBLK, "Block device required"),
    ERRNAME(EBUSY, "Device or resource busy"),
    ERRNAME(EEXIST, "File exists"),
    ERRNAME(EXDEV, "Cross-device link"),
    ERRNAME(ENODEV, "No such device"),
    ERRNAME(ENOTDIR, "Not a directory"),
    ERRNAME(EISDIR, "Is a directory"),
    ERRNAME(EINVAL, "Invalid argument"),
    ERRNAME(ENFILE, "File table overflow"),
    ERRNAME(EMFILE, "Too many open files"),
    ERRNAME(ENOTTY, "Not a typewriter"),
    ERRNAME(ETXTBSY, "Text file busy"),
    ERRNAME(EFBIG, "File too large"),
    ERRNAME(ENOSPC, "No space left on device"),
    ERRNAME(ESPIPE, "Illegal seek"),
    ERRNAME(EROFS, "Read-only file system"),
    ERRNAME(EMLINK, "Too many links"),
    ERRNAME(EPIPE, "Broken pipe"),
    ERRNAME(EDOM, "Math argument out of domain of func"),
    ERRNAME(ERANGE, "Math result not representable"),

    ERRNAME(EDEADLK, "Resource deadlock would occur"),
    ERRNAME(ENAMETOOLONG, "File name too long"),
    ERRNAME(ENOLCK, "No record locks available"),
    ERRNAME(ENOSYS, "Invalid system call number"),
    ERRNAME(EBFONT, "Bad font file format"),
    ERRNAME(ENOSTR, "Device not a stream"),
    ERRNAME(ENODATA, "No data available"),
    ERRNAME(ETIME, "Timer expired"),
    ERRNAME(ENOSR, "Out of streams resources"),
    ERRNAME(ENONET, "Machine is not on the network"),
    ERRNAME(ENOPKG, "Package not installed"),
    ERRNAME(EREMOTE, "Object is remote"),
    ERRNAME(ENOLINK, "Link has been severed"),
    ERRNAME(EADV, "Advertise error"),
    ERRNAME(ESRMNT, "Srmount error"),
    ERRNAME(ECOMM, "Communication error on send"),
    ERRNAME(EPROTO, "Protocol error"),
    ERRNAME(EMULTIHOP, "Multihop attempted"),
    ERRNAME(EDOTDOT, "RFS specific error"),
    ERRNAME(EBADMSG, "Not a data message"),
    ERRNAME(EOVERFLOW, "Value too large for defined data type"),
    ERRNAME(ENOTUNIQ, "Name not unique on network"),
    ERRNAME(EBADFD, "File descriptor in bad state"),
    ERRNAME(EREMCHG, "Remote address changed"),
    ERRNAME(ELIBACC, "Can not access a needed shared library"),
    ERRNAME(ELIBBAD, "Accessing a corrupted shared library"),
    ERRNAME(ELIBSCN, ".lib section in a.out corrupted"),
    ERRNAME(ELIBMAX, "Attempting to link in too many shared libraries"),
    ERRNAME(ELIBEXEC, "Cannot exec a shared library directly"),
    ERRNAME(EILSEQ, "Illegal byte sequence"),
    ERRNAME(ERESTART, "Interrupted system call should be restarted"),
    ERRNAME(ESTRPIPE, "Streams pipe error"),
    ERRNAME(EUSERS, "Too many users"),
    ERRNAME(ENOTSOCK, "Socket operation on non-socket"),
    ERRNAME(EDESTADDRREQ, "Destination address required"),
    ERRNAME(EMSGSIZE, "Message too long"),
    ERRNAME(EPROTOTYPE, "Protocol wrong type for socket"),
    ERRNAME(ENOPROTOOPT, "Protocol not available"),
    ERRNAME(EPROTONOSUPPORT, "Protocol not supported"),
    ERRNAME(ESOCKTNOSUPPORT, "Socket type not supported"),
    ERRNAME(EOPNOTSUPP, "Operation not supported on transport endpoint"),
    ERRNAME(EPFNOSUPPORT, "Protocol family not supported"),
    ERRNAME(EAFNOSUPPORT, "Address family not supported by protocol"),
    ERRNAME(EADDRINUSE, "Address already in use"),
    ERRNAME(EADDRNOTAVAIL, "Cannot assign requested address"),
    ERRNAME(ENETDOWN, "Network is down"),
    ERRNAME(ENETUNREACH, "Network is unreachable"),
    ERRNAME(ENETRESET, "Network dropped connection because of reset"),
    ERRNAME(ECONNABORTED, "Software caused connection abort"),
    ERRNAME(ECONNRESET, "Connection reset by peer"),
    ERRNAME(ENOBUFS, "No buffer space available"),
    ERRNAME(EISCONN, "Transport endpoint is already connected"),
    ERRNAME(ENOTCONN, "Transport endpoint is not connected"),
    ERRNAME(ESHUTDOWN, "Cannot send after transport endpoint shutdown"),
    ERRNAME(ETOOMANYREFS, "Too many references: cannot splice"),
    ERRNAME(ETIMEDOUT, "Connection timed out"),
    ERRNAME(ECONNREFUSED, "Connection refused"),
    ERRNAME(EHOSTDOWN, "Host is down"),
    ERRNAME(EHOSTUNREACH, "No route to host"),
    ERRNAME(EALREADY, "Operation already in progress"),
    ERRNAME(EINPROGRESS, "Operation now in progress"),
    ERRNAME(ESTALE, "Stale file handle"),
    ERRNAME(EUCLEAN, "Structure needs cleaning"),
    ERRNAME(ENOTNAM, "Not a XENIX named type file"),
    ERRNAME(ENAVAIL, "No XENIX semaphores available"),
    ERRNAME(EISNAM, "Is a named type file"),
    ERRNAME(EREMOTEIO, "Remote I/O error"),
    ERRNAME(EDQUOT, "Quota exceeded"),
    ERRNAME(ENOMEDIUM, "No medium found"),
    ERRNAME(EMEDIUMTYPE, "Wrong medium type"),
    ERRNAME(ECANCELED, "Operation Canceled"),
    ERRNAME(ENOKEY, "Required key not available"),
    ERRNAME(EKEYEXPIRED, "Key has expired"),
    ERRNAME(EKEYREVOKED, "Key has been revoked"),
    ERRNAME(EKEYREJECTED, "Key was rejected by service"),

    { }, /* NULL */
};

static long
errname_search(const void *key, void *pdata)
{
    const struct bfdev_errname *entry = key;
    return entry->errnum - (long)pdata;
}

export struct bfdev_errname *
bfdev_errname_find(int error)
{
    struct bfdev_errname *entry;

    if (unlikely(error > 0))
        return NULL;

    entry = bfdev_bsearch(
        bfdev_errname_table, ARRAY_SIZE(bfdev_errname_table) - 1,
        sizeof(*entry), errname_search, (void *)(long)-error
    );

    return entry;
}

export const char *
bfdev_errname(int error, const char **infop)
{
    struct bfdev_errname *entry;

    entry = bfdev_errname_find(error);
    if (entry && infop)
        *infop = entry->info;

    return entry ? entry->name : NULL;
}
