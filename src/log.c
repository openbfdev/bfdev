#include <bfdev/allocator.h>
#include <bfdev/log.h>
#include <bfdev/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <bfdev/scnprintf.h>

static const unsigned int
bfdev_level_color[] = {
    [BFDEV_LOG_EMERG]     = BFDEV_COLR_RED,
    [BFDEV_LOG_ALERT]     = BFDEV_COLR_DARK_MAGENTA,
    [BFDEV_LOG_CRIT]      = BFDEV_COLR_MAGENTA,
    [BFDEV_LOG_ERR]       = BFDEV_COLR_YELLOW,
    [BFDEV_LOG_WARNING]   = BFDEV_COLR_BLUE,
    [BFDEV_LOG_NOTICE]    = BFDEV_COLR_CYAN,
    [BFDEV_LOG_INFO]      = BFDEV_COLR_GREEN,
    [BFDEV_LOG_DEBUG]     = BFDEV_COLR_DARK_GRAY,
    [BFDEV_LOG_DEFAULT]   = BFDEV_COLR_DEFAULT,
};

static char *bfdev_log_levels[] = {
    "emerg",
    "alert",
    "crit",
    "error",
    "warn",
    "notice",
    "info",
    "debug"
};


int bfdev_log_init(bfdev_log_t *log, unsigned log_level) {
    if (log == NULL) {
        return BFDEV_ENOMEM;
    }

    log->log_level = log_level;
    log->writer = bfdev_stderr_writer;
    log->pdata = NULL;
    return BFDEV_ENOERR;
}

int bfdev_log_init_writer(bfdev_log_t *log, bfdev_log_writer_pt writer, void *pdata) {
    if (log == NULL) {
        return BFDEV_ENOMEM;
    }

    log->writer = writer;
    log->pdata = pdata;

    return BFDEV_ENOERR;
}

void bfdev_log_level_print(unsigned level, bfdev_log_t *log, const char *fmt, ...) {
    char       errstr[BFDEV_MAX_LOG_STR];
    char *p, *last;
    int len = 0;
    va_list      args;
    bfdev_log_meta_info_t *minfo;

    if (log == NULL) {
        return;
    }

    p = errstr;
    last = p + BFDEV_MAX_LOG_STR;

    len = snprintf(p + len, last - p, "\e[%dm", bfdev_level_color[level]);
    p+=len;

    len = snprintf(p, last - p, "[%s] ", bfdev_log_levels[level]);
    p+=len;

    for (minfo = log->minfo; minfo; minfo = minfo->next) {
        if (minfo->handler) {
            len = minfo->handler(log, level, p, last - p, minfo->data);
            p+=len;
        }
    }

    va_start(args, fmt);
    len = bfdev_vscnprintf(p, last - p, fmt, args);
    p+=len;
    va_end(args);

    //TODO: use meta data heandler to replace it
    len = snprintf(p, last - p, "\e[0m");
    p+=len;

    if (p > last - BFDEV_LINEFEED_SIZE) {
        p = last - BFDEV_LINEFEED_SIZE;
    }

    bfdev_linefeed(p);

    if (log->writer) {
        return log->writer(log, level, errstr, p - errstr);
    }

    return bfdev_stderr_writer(log, level, errstr, p - errstr);
}

int bfdev_log_meta_info_add(bfdev_log_t *log, bfdev_log_meta_info_t *minfo) {
    if (log == NULL || minfo == NULL) {
        return BFDEV_ENOMEM;
    }
    
    minfo->next = log->minfo;

    log->minfo = minfo;

    return BFDEV_ENOERR;
} 

void bfdev_stderr_writer(bfdev_log_t *log, unsigned level, char *buf, size_t len) {
    ssize_t n;
    (void) level, (void)log;

    n = write(STDERR_FILENO, buf, len);
    if (n != len) {
        perror("bfdev stderr write error");
        return;
    }
    return;
}