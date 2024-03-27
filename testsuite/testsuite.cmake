# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

set(TESTSUITE_DIR
    ${CMAKE_CURRENT_LIST_DIR}
)

set(TESTSUITE_SOURCE
    ${TESTSUITE_DIR}/testsuite.c
    ${TESTSUITE_DIR}/testsuite.h
)

function(testsuite_target name src dest)
    string(
        REGEX REPLACE ${src} ""
        TESTSUIT_OBJECT "${BFDEV_LIBRARY}"
    )

    add_library(testsuit_object OBJECT ${dest})
    add_executable("${name}" ${TESTSUITE_SOURCE} ${TESTSUIT_OBJECT})
    target_link_libraries("${name}" testsuit_object)

    target_compile_options(
        testsuit_object PRIVATE
        -include ${src}
        -I ${TESTSUITE_DIR}
    )

    target_compile_options(
        "${name}" PRIVATE
        -I ${TESTSUITE_DIR}
    )
endfunction()
