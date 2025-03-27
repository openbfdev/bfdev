# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

if(BFDEV_ASAN)
    set(BFDEV_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=address \
         -fsanitize=undefined \
         -fsanitize-recover=all \
         -fno-omit-frame-pointer \
         -fno-stack-protector"
    )
    if(NOT APPLE)
        set(BFDEV_C_FLAGS
            "${BFDEV_C_FLAGS} \
             -fsanitize=leak"
        )
    endif()
endif()

if(BFDEV_UBSAN)
    set(BFDEV_C_FLAGS
        "${BFDEV_C_FLAGS} \
         -fsanitize=alignment \
         -fsanitize=bounds \
         -fsanitize=shift \
         -fsanitize=integer-divide-by-zero \
         -fsanitize=unreachable \
         -fsanitize=bool \
         -fsanitize=enum"
    )
endif()

if(BFDEV_GCOV)
    set(BFDEV_C_FLAGS
        "${BFDEV_C_FLAGS} \
         -fprofile-arcs \
         -ftest-coverage"
    )
endif()
