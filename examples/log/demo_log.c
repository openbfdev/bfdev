#include <bfdev/log.h>
#include <bfdev/errno.h>
#include <stddef.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

static pid_t bfdev_demo_pid;
int log_append_time(bfdev_log_t *log, unsigned level, char *buf, size_t size, void *udata);
int log_append_pid(bfdev_log_t *log, unsigned level, char *buf, size_t size, void *udata);

bfdev_log_meta_info_t minfo_time = {
    .data = NULL,
    .handler = log_append_time,
    .next = NULL
};

bfdev_log_meta_info_t minfo_pid = {
    .data = NULL,
    .handler = log_append_pid,
    .next = NULL
};

void
bfdev_localtime(time_t s, struct tm *tm)
{
#if (HAVE_LOCALTIME_R)
    (void) localtime_r(&s, tm);

#else
    struct tm       *t;

    t = localtime(&s);
    *tm = *t;

#endif

    tm->tm_mon++;
    tm->tm_year += 1900;
}

int log_append_time(bfdev_log_t *log, unsigned level, char *buf, size_t size, void *udata) {
    struct timeval   tv;
    time_t           sec;
    struct tm        tm;

    (void)gettimeofday(&tv, NULL);

    sec = tv.tv_sec;
    bfdev_localtime(sec, &tm);
    //format "1970/09/28 12:00:00"
    return snprintf(buf, size, "[%4d/%02d/%02d %02d:%02d:%02d] ",
                    tm.tm_year, tm.tm_mon,
                    tm.tm_mday, tm.tm_hour,
                    tm.tm_min, tm.tm_sec);
}

int log_append_pid(bfdev_log_t *log, unsigned level, char *buf, size_t size, void *udata) {
    return snprintf(buf, size, "pid=%ld ", (long)bfdev_demo_pid);
}


int bfdev_demo_init() {
    bfdev_demo_pid = getpid();
    return BFDEV_ENOERR;
}
int	main(int argc, char **argv) {
    bfdev_log_t log;
    if (bfdev_demo_init() != BFDEV_ENOERR) {
        return -1;
    }

    if (bfdev_log_init(&log, BFDEV_LOG_EMERG) != BFDEV_ENOERR) {
        return -1;
    }

    if (bfdev_log_meta_info_add(&log, &minfo_time) != BFDEV_ENOERR) {
        return -1;
    }

    if (bfdev_log_meta_info_add(&log, &minfo_pid) != BFDEV_ENOERR) {
        return -1;
    }
    
    bfdev_log(BFDEV_LOG_EMERG, &log, "hello world");
    bfdev_log(BFDEV_LOG_ALERT, &log, "hello world");
    bfdev_log(BFDEV_LOG_CRIT, &log, "hello world");
    bfdev_log(BFDEV_LOG_ERR, &log, "hello world");
    bfdev_log(BFDEV_LOG_WARNING, &log, "hello world");
    bfdev_log(BFDEV_LOG_NOTICE, &log, "hello world");
    bfdev_log(BFDEV_LOG_INFO, &log, "hello world");
    bfdev_log(BFDEV_LOG_DEBUG, &log, "hello world");

    log.log_level = BFDEV_LOG_DEBUG;
    bfdev_log_emerg(&log, "hello world");
    bfdev_log_alert(&log, "hello world");
    bfdev_log_crit(&log, "hello world");
    bfdev_log_err(&log, "hello world");
    bfdev_log_warning(&log, "hello world");
    bfdev_log_notice(&log, "hello world");
    bfdev_log_info(&log, "hello world");
    bfdev_log_debug(&log, "hello world");


}
