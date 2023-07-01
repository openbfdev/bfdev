#ifndef __LOG__H__
#define __LOG__H__

#include <stddef.h>
#include <bfdev/allocator.h>
typedef struct bfdev_log_s bfdev_log_t;

#define BFDEV_LOG_EMERG           0
#define BFDEV_LOG_ALERT           1
#define BFDEV_LOG_CRIT            2
#define BFDEV_LOG_ERR             3
#define BFDEV_LOG_WARNING         4
#define BFDEV_LOG_NOTICE          5
#define BFDEV_LOG_INFO            6
#define BFDEV_LOG_DEBUG           7
#define BFDEV_LOG_DEFAULT         8

#define BFDEV_COLR_DEFAULT          39
#define BFDEV_COLR_BLACK            30
#define BFDEV_COLR_DARK_RED         31
#define BFDEV_COLR_DARK_GREEN       32
#define BFDEV_COLR_DARK_YELLOW      33
#define BFDEV_COLR_DARK_BLUE        34
#define BFDEV_COLR_DARK_MAGENTA     35
#define BFDEV_COLR_DARK_CYAN        36
#define BFDEV_COLR_LIGHT_GRAY       37
#define BFDEV_COLR_DARK_GRAY        90
#define BFDEV_COLR_RED              91
#define BFDEV_COLR_GREEN            92
#define BFDEV_COLR_YELLOW           93
#define BFDEV_COLR_BLUE             94
#define BFDEV_COLR_MAGENTA          95
#define BFDEV_COLR_CYAN             96
#define BFDEV_COLR_WHITE            97


#define BFDEV_MAX_LOG_STR 2048

// FIXME: configure it
#define LF     (char) '\n'

#define BFDEV_LINEFEED_SIZE 1
#define bfdev_linefeed(p)          *p++ = LF;

typedef void (*bfdev_log_writer_pt) (bfdev_log_t *log, unsigned level,
    char *buf, size_t len);


typedef int (*bfdev_log_meta_info_pt)(bfdev_log_t *log, unsigned level, char *buf, size_t len, void *udata);

typedef struct bfdev_log_meta_info_s bfdev_log_meta_info_t;
struct bfdev_log_s {
    unsigned log_level;
    bfdev_log_writer_pt writer;
    void *pdata;
    bfdev_log_meta_info_t *minfo;
};

struct bfdev_log_meta_info_s {
    bfdev_log_meta_info_pt handler;
    void *data;
    bfdev_log_meta_info_t *next;
};

#define bfdev_log(level, log, ...)                                        \
    if ((log)->log_level >= level) bfdev_log_level_print(level, log, __VA_ARGS__)

#define bfdev_log_emerg(log, ...)                                        \
    bfdev_log(BFDEV_LOG_EMERG, log, __VA_ARGS__)

#define bfdev_log_alert(log, ...)                                        \
    bfdev_log(BFDEV_LOG_ALERT, log, __VA_ARGS__)

#define bfdev_log_crit(log, ...)                                        \
    bfdev_log(BFDEV_LOG_CRIT, log, __VA_ARGS__)

#define bfdev_log_err(log, ...)                                        \
        bfdev_log(BFDEV_LOG_ERR, log, __VA_ARGS__)

#define bfdev_log_warning(log, ...)                                        \
    bfdev_log(BFDEV_LOG_WARNING, log, __VA_ARGS__)

#define bfdev_log_notice(log, ...)                                        \
    bfdev_log(BFDEV_LOG_NOTICE, log, __VA_ARGS__)

#define bfdev_log_info(log, ...)                                        \
    bfdev_log(BFDEV_LOG_INFO, log, __VA_ARGS__)

#define bfdev_log_debug(log, ...)                                        \
    bfdev_log(BFDEV_LOG_DEBUG, log, __VA_ARGS__)


int bfdev_log_init(bfdev_log_t *log, unsigned level);
void bfdev_stderr_writer(bfdev_log_t *log, unsigned level, char *buf, size_t len);
void bfdev_log_level_print(unsigned level, bfdev_log_t *log, const char *fmt, ...);
int bfdev_log_meta_info_add(bfdev_log_t *log, bfdev_log_meta_info_t *minfo);



#endif  /*__LOG__H__*/