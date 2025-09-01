# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
#

set(BFDEV_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/chain-hostname.c
    ${CMAKE_CURRENT_LIST_DIR}/chain-localtime.c
    ${CMAKE_CURRENT_LIST_DIR}/chain-pid.c
    ${CMAKE_CURRENT_LIST_DIR}/write-syslog.c
)

check_header_and_exit("time.h")
check_header_and_exit("unistd.h")
check_header_and_exit("syslog.h")
check_header_and_exit("sys/utsname.h")

check_symbol_and_exit("time.h" "time")
check_symbol_and_exit("time.h" "localtime")
check_symbol_and_exit("time.h" "strftime")
check_symbol_and_exit("unistd.h" "getpid")
check_symbol_and_exit("syslog.h" "syslog")
check_symbol_and_exit("sys/utsname.h" "uname")
