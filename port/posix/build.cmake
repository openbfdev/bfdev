# SPDX-License-Identifier: LGPL-3.0-or-later
#
# Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
#

set(BFDEV_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/log.c
    ${CMAKE_CURRENT_LIST_DIR}/stdio.c
    ${CMAKE_CURRENT_LIST_DIR}/stdlib.c
    ${CMAKE_CURRENT_LIST_DIR}/string.c
)

check_header_and_exit("stdio.h")
check_header_and_exit("stdlib.h")
check_header_and_exit("string.h")

check_type_and_exit("char" "1")
check_type_and_exit("short" "2")
check_type_and_exit("int" "4")
check_type_and_exit("long long" "8")

# stdio
check_symbol_and_exit("stdio.h" "fwrite")
check_symbol_and_exit("stdio.h" "fflush")
check_symbol_and_exit("stdio.h" "vsnprintf")

# stdlib
check_symbol_and_exit("stdlib.h" "malloc")
check_symbol_and_exit("stdlib.h" "calloc")
check_symbol_and_exit("stdlib.h" "realloc")
check_symbol_and_exit("stdlib.h" "free")
check_symbol_and_exit("stdlib.h" "rand")
check_symbol_and_exit("stdlib.h" "abort")

# string
check_symbol_and_exit("string.h" "memcpy")
check_symbol_and_exit("string.h" "memset")
check_symbol_and_exit("string.h" "memcmp")
check_symbol_and_exit("string.h" "strcmp")
check_symbol_and_exit("string.h" "strchr")
check_symbol_and_exit("string.h" "strspn")
check_symbol_and_exit("string.h" "strcspn")
check_symbol_and_exit("string.h" "strcpy")
check_symbol_and_exit("string.h" "strncpy")
check_symbol_and_exit("string.h" "strlen")
check_symbol_and_exit("string.h" "strnlen")
