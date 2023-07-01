#include <bfdev/log.h>
#include <bfdev/errno.h>

int	main(int argc, char **argv) {
    bfdev_log_t log;
    if (bfdev_log_init(&log, BFDEV_LOG_EMERG) != BFDEV_ENOERR) {
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

    return 0;
}